#pragma once

#include "c_irc.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Message.hpp"

#include <string>

namespace c_irc
{
	class Server
	{
		#define LOG_USER(fd, msg) std::cout << "User [id=" << fd << "] : " << msg << std::endl;

		private:

		typedef void (Server::*cmd_ptr)(int, arguments_t &);
		typedef std::map<std::string, cmd_ptr>	commands_t;

		std::string		name;

		int				server_fd;

		uint16_t		port;
		std::string		password;

		pollfds_t		pollfds;
		serv_users_t	users;

		channels_t		channels;
		messages_t		buffer;

		commands_t		commands;
		// Logger		*log;

		std::string		creation_time;
		std::string		compilation_time;

		public:

		Server();
		~Server();

		void			initialize(	std::string new_name,
							std::string new_port,
							std::string new_password);

		void			start();

		private:

		void			accept_connections();
		void			check_all_clients(int rc);

		void			send_message(c_irc::Message *msg, pollfd &pfd);
		void			create_channel(std::string name, int user, std::string key);
		void			delete_channel(std::string name);
		void			delete_user(int fd);
		void			delete_user(int index, int fd);

		void			parse_message(std::string msg, int fd);
		void			execute_command(c_irc::Command &cmd, int fd);
		void			init_commands();
		void			queue_message(std::string msg, int fd);
		void			queue_message(std::string payload, c_irc::Channel *chan);
		void			queue_message(std::string payload, c_irc::Channel *chan, int fd);

		std::string		get_password() const;

		int				is_user(std::string name);

		void			check_user_quit();
		void			delete_empty_channels();

		/*
		 * Commands
		 */

		void			cmd_nick(int fd, arguments_t &args);
		void			cmd_pass(int fd, arguments_t &args);
		void			cmd_user(int fd, arguments_t &args);
		void			cmd_cap(int fd, arguments_t &args);

		void			cmd_oper(int fd, arguments_t &args);
		void			cmd_mode(int fd, arguments_t &args);
		void			cmd_quit(int fd, arguments_t &args);
		void			cmd_kick(int fd, arguments_t &args);
		void			cmd_join(int fd, arguments_t &args);
		void			cmd_part(int fd, arguments_t &args);
		void			cmd_topic(int fd, arguments_t &args);
		void			cmd_names(int fd, arguments_t &args);
		void			cmd_list(int fd, arguments_t &args);
		void			cmd_privmsg(int fd, arguments_t &args);
		void			cmd_kill(int fd, arguments_t &args);
		void			cmd_ping(int fd, arguments_t &args);
		void			cmd_notice(int fd, arguments_t &args);
		void			cmd_invite(int fd, arguments_t &args);
		void			cmd_time(int fd, arguments_t &args);

		void			cmd_pong(int fd, arguments_t &args);
		void			cmd_motd(int fd, arguments_t &args);
		void			cmd_info(int fd, arguments_t &args);
		void			cmd_version(int fd, arguments_t &args);

		void			welcome(int fd);


		/* 
		 * cmd_utils
		 */

		void			print_channel_list(Channel *chan, int fd, std::string nick);
		std::string		print_users_list(Channel *chan, int fd, std::string nick); 
		std::string		print_users_list(Channel *chan, int fd, std::string nick, std::set<int> &users_in_chan); 
		void			parse_cmd_join(arguments_t &args, int fd, std::string chan_name); 
		
		bool			channel_autorization(std::string element, int fd, std::string key); 

		void			cmd_mode_chan(int fd, c_irc::User &user, arguments_t &args);
		void			cmd_mode_user(int fd, c_irc::User &user, arguments_t &args);

	};
} // namespace c_irc
