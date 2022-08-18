#include "Server.hpp"
#include "Message.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#define LOG(x) std::cout << x << std::endl;

// temp

void signal_handler(int signum)
{
	LOG("\nSignal " << signum << " received");
}

namespace c_irc
{
	Server::Server()
	{}

	Server::~Server()
	{}

	void	Server::initialize(	std::string new_name,
								std::string new_ip,
								std::string new_port)
	{
		int rc;

		name = new_name;
		ip = inet_addr(new_ip.c_str());
		port = htons(std::stoi(new_port));	// replace stoi

		/*
		 *	Socket creation
		 */

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			throw std::runtime_error("socket() failed to open");

		LOG("Socket created");

		fcntl(fd, F_SETFL, O_NONBLOCK);

		/*
		 *	Socket binding
		 */

		struct sockaddr_in server_addr;

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = port;
		server_addr.sin_addr.s_addr = ip;	
		
		rc = bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (rc < 0) {
			close(fd);
			throw std::runtime_error("bind() failed to open");
		}

		LOG("Socket binded");

		/*
		 *	Listening socket
		 */

		rc = listen(fd, 42);
		if (rc < 0) {
			close(fd);
			throw std::runtime_error("listen() failed to open");
		}

		pollfd pfd;

		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		pollfds.push_back(pfd);

		LOG("listen() success");
		LOG("Listening on port: " << ntohs(server_addr.sin_port));
	}

	void	Server::start()
	{
		// Change signal behavior
		signal(SIGINT, &signal_handler);
		signal(SIGTERM, &signal_handler);
		signal(SIGQUIT, &signal_handler);

		while (0xCAFE) {
			if (not buffer.empty()) {
				buffer.front()->prepare_message();
			}
		}
	}
} // namespace c_irc
