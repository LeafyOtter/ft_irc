#include "Server.hpp"
#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>

#include <errno.h>

#include <algorithm>

#define STOI(x) (atoi(x.c_str()))

void signal_handler(int signum)
{
	std::cout << "\nSignal " << signum << " received" << std::endl;
	exit(1);
}

namespace c_irc
{
	Server::Server(	std::string const& port,
					std::string const& address,
					std::string const& password )
		: password(password)
		, port(STOI(port))
		, addr(STOI(address))
		// , fd(0)
		// , sa()
		// , fds()
	{}

	Server::~Server()
	{
		close(fd);
		close_all_connections();
		fds.clear();
	}

	void	Server::initialize()
	{
		pollfd pfd;

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			throw std::runtime_error("socket() failed");
		fcntl(fd, F_SETFL, O_NONBLOCK);
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(fd, (sockaddr*)&sa, sizeof(sa)) == -1)
			throw std::runtime_error("bind() failed");
		if (listen(fd, 42) == -1)
			throw std::runtime_error("listen() failed");
		pfd.fd = fd;
		pfd.events = POLLIN;
		fds.push_back(pfd);

		std::cout << "Server initialized" << std::endl;
		std::cout << "Listening on port " << port << std::endl;
	}

	void	Server::run()
	{
		int		rc;
		size_t	size;

		fds.reserve(42);

		signal(SIGINT, &signal_handler);
		while (0xCAFE)
		{
			rc = poll(&fds[0], fds.size(), -1);
			if (rc == -1) {
				if (errno == EINTR)
					break ;
				throw std::runtime_error("poll() failed");
			}
			if (fds[0].revents)	{// new user
				accept_new_connections();
				continue ;
			}

			if (rc)
			{
				size = fds.size();

				for (size_t i = 0; i < size; ++i)
				{
					pollfd &pfd = fds[i];

					if (!pfd.revents)
						continue ;
					if (!(pfd.revents & POLLIN))
						throw std::runtime_error("poll() failed");
					if (pfd.fd > 0)
						read_from_client(pfd, i);
					else
						std::cerr << "error read" << std::endl;
				}
			}
			
		}
	}

	// private methods

	void	Server::accept_new_connections()
	{
		pollfd		pfd;
		int			new_fd;
		sockaddr_in	new_sa;
		socklen_t	new_sa_len;

		new_sa_len = sizeof(new_sa);
		new_fd = accept(fd, (sockaddr*)&new_sa, &new_sa_len);
		if (new_fd == -1)
			close(fd), throw std::runtime_error("accept() failed");
		std::cout << "New connection from " << inet_ntoa(new_sa.sin_addr) << std::endl;
		pfd.fd = new_fd;
		pfd.events = POLLIN;
		fds.push_back(pfd);
	}

	void	Server::read_from_client(pollfd &pfd, int index)
	{
		int		rc;
		char	buffer[1024];

		// memset(buffer, 0, sizeof(buffer));
		std::fill(buffer, buffer + sizeof(buffer), 0); // equivalent to memset (just in a c++ way)
		rc = recv(pfd.fd, buffer, sizeof(buffer) - 1, 0);

		if (!rc)
		{
			std::cout << "Client " << pfd.fd << " disconnected" << std::endl;
			close(pfd.fd);
			fds.erase(fds.begin() + index);
			return ;
		}

		buffer[rc + 1] = '\0';
		std::cout << "Client " << pfd.fd << ": " << buffer;
		send(pfd.fd, buffer, rc, 0);
	}
	
	void	Server::close_all_connections()
	{
		for (size_t i = 1; i < fds.size(); ++i)
			close(fds[i].fd);
	}
} // namespace c_irc
