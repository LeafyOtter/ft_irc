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

		int				fd;

		uint16_t		port;
		std::string		password;

		pollfds_t		pollfds;
		serv_users_t	users;

		channels_t		channels;
		messages_t		buffer;

		commands_t		commands;
		// Logger		*log;

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
		void			create_channel(std::string name, int user);
		void			delete_channel(std::string name);
		void			delete_user(int index, int fd);

		void			parse_message(std::string msg, int fd);
		void			execute_command(c_irc::Command &cmd, int fd);
		void			init_commands();
		void			queue_message(std::string msg, int fd);
		void			queue_message(std::string msg, chan_users_it_t first, chan_users_it_t last);

		std::string		get_password() const;

		/*
		 * Commands
		 */

		void			cmd_nick(int fd, arguments_t &args);
		void			cmd_pass(int fd, arguments_t &args);
		void			cmd_user(int fd, arguments_t &args);
		void			cmd_cap(int fd, arguments_t &args);
	};
} // namespace c_irc
