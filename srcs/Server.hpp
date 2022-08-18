#pragma once

#include "c_irc.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <string>

typedef std::queue<c_irc::Message *>	messages_t;
typedef std::vector<pollfd>				pollfds_t;

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

		messages_t		buffer;

		// Logger		*log;

		public:

		Server();
		~Server();

		void	initialize(	std::string new_name,
							std::string new_ip,
							std::string port);

		void	start();

	};
} // namespace c_irc
