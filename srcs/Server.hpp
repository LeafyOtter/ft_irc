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

		users_t			users;
		pollfds_t		pollfds;

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
		void	accept_connections();
		void	check_all_clients(int rc);

		users_it_t	find_user(int fd);

		void	send_message(c_irc::Message *msg, pollfd &pfd);
		void	create_channel(std::string name, User *user);
		void	delete_channel(std::string name);
	};
} // namespace c_irc
