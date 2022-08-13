#pragma once

#include "c_irc.hpp"

#include <string>

namespace c_irc
{
	class Server
	{
		private:

		std::string		name;
		std::string		host;

		int				fd;

		uint16_t		port;

		users_t			users;


		public:

		Server(/* args */);
		~Server();
	};
} // namespace c_irc
