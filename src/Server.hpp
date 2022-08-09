#pragma once

#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

namespace c_irc
{
	class Server
	{
		public :

		typedef struct sockaddr_in	sockaddr_in;

		Server(	std::string const& port,
				std::string const& addr,
				std::string const& password );
		~Server();

		void initialize();
		void run();

		private :

		const std::string	password;
		const uint16_t		port;
		const in_addr_t		addr;
		int					fd;
		sockaddr_in			sa;
		std::vector<pollfd>	fds;

		void accept_new_connections();
		void read_from_client(pollfd &pfd, int index);

		Server();
		Server(Server const& other);
		Server& operator=(Server const& other);

	};
} // namespace c_irc
