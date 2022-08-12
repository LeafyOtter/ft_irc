#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <vector>

int main(int ac, char **av)
{
	int	socket_fd;

	if (ac != 2)
	{
		std::cout << "Usage : " << av[0] << " <port>" << std::endl;
		return (1);
	}

	/* 
	 * Socket creation
	 */

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket() failed");
		return (1);
	}

	/*
	 * Non blocking socket
	 */

	fcntl(socket_fd, F_SETFL, O_NONBLOCK);

	/* 
	 * Socket binding
	 */

	struct sockaddr_in server_addr;

	// Protocol family
	server_addr.sin_family = AF_INET;
	// Port number
	server_addr.sin_port = htons(atoi(av[1]));
	// Any interface
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding

	int bind_return;

	bind_return = bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (bind_return == -1) {
		perror("bind() failed");
		close(socket_fd);
		return (1);
	}

	std::cout << "bind() success" << std::endl;

	/* 
	 * Socket listening
	 */

	std::cout << "Listening on port: " << ntohs(server_addr.sin_port) << std::endl;

	int listen_return;

	listen_return = listen(socket_fd, 1024);
	if (listen_return == -1) {
		perror("listen() failed");
		close(socket_fd);
		return (1);
	}

	std::cout << "listen() success" << std::endl;

	/*
	 * Initialize pollfd vector
	 */

	pollfd pfd_0;

	pfd_0.fd = socket_fd;
	pfd_0.events = POLLIN;

	std::vector<pollfd> pfds;

	pfds.push_back(pfd_0);

	/*
	 * Main loop
	 */

	char buffer[1024];

	while (true)
	{
		pfds.reserve(42);

		int poll_return;

		poll_return = poll(&pfds[0], pfds.size(), -1);

		if (pfds[0].revents)
		{
			int client_fd;
			struct sockaddr_in client_addr;
			socklen_t client_addr_len;
			client_addr_len = sizeof(client_addr);
			client_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);
			if (client_fd == -1) {
				perror("accept() failed");
				close(socket_fd);
				return (1);
			}
			std::cout << "Client " << client_fd << " connected" << std::endl;
			pollfd pfd_1;
			pfd_1.fd = client_fd;
			pfd_1.events = POLLIN;
			pfds.push_back(pfd_1);
			continue ;
		}

		if (poll_return)
		{
			size_t size = pfds.size();

			for (size_t i = 0; i < size; i++)
			{
				pollfd &pfd = pfds[i];

				if (!pfd.revents)
					continue ;
				if (!(pfd.revents & POLLIN))
					perror("poll revent is set wrong");
				if (pfd.fd > 0)
				{
					int recv_return;

					memset(buffer, 0, sizeof(buffer));
					recv_return = recv(pfd.fd, buffer, sizeof(buffer), 0);
					if (!recv_return)
					{
						std::cout << "Client " << pfd.fd << " disconnected" << std::endl;
						close(pfd.fd);
						pfds.erase(pfds.begin() + i);
						continue ;
					}

					std::cout << "Client " << pfd.fd << " sent a message" << std::endl;

					std::string str = "Client " + std::to_string(pfd.fd) + ": " + buffer;

					std::cout << "debug : " << str;

					size_t _size = pfds.size();
					for (size_t j = 1; j < _size; j++)
					{
						std::cout << "Forwarding message to client " << pfds[j].fd << std::endl;
						if (pfds[j].fd <= 0)
							break ;
						send(pfds[j].fd, str.c_str(), strlen(str.c_str()), 0);
					}
				}
				else
					perror("tried to recv on non-existing client");
			}
		}
	}
	close(socket_fd);
}
