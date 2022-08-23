#pragma once

#include "c_irc.hpp"
#include "Message.hpp"
#include "Channel.hpp"

#include <string>

namespace c_irc
{
	class Server
	{
		private:

		std::string		name;

		int				fd;

		uint16_t		ip;
		uint16_t		port;

		pollfds_t		pollfds;
		serv_users_t	users;

		channels_t		channels;
		messages_t		buffer;

		// Logger		*log;

		public:

		Server();
		~Server();

		void	initialize(	std::string new_name,
							std::string new_ip,
							std::string port);

		void	start();

		private:

		void	accept_connections();
		void	check_all_clients(int rc);

		void	send_message(c_irc::Message *msg, pollfd &pfd);
		void	create_channel(std::string name, int user);
		void	delete_channel(std::string name);
		void	delete_user(int index, int fd);
	};
} // namespace c_irc
