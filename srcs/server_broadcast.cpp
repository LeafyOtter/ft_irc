#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#include <fcntl.h>

#include <unistd.h>

#include <signal.h>

void sub_to_pollout(std::vector<struct pollfd> &pfds, size_t i);

#define LOGGER(x) std::cout << x << std::endl;

void signal_handler(int signum)
{
	LOGGER("\nSignal " << signum << " received");
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage : " << av[0] << " <port> <path>" << std::endl;
		return (1);
	}

	/**
	 * Socket creation
	 */

	int server_fd;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket() failed");
		return (1);
	}

	LOGGER("Socket created");

	/**
	 * Non blocking socket
	 */

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	/**
	 * Socket binding
	 */

	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(av[1]));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/**
	 * Binding
	 */

	int rc;

	rc = bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (rc == -1) {
		perror("bind() failed");
		close(server_fd);
		return (1);
	}

	LOGGER("Socket binded");

	/**
	 * Listen
	 */

	rc = listen(server_fd, 42);
	if (rc == -1) {
		perror("listen() failed");
		close(server_fd);
		return (1);
	}

	LOGGER("listen() success");
	LOGGER("Listening on port: " << ntohs(server_addr.sin_port));

	/**
	 * Initialize pollfd
	 */

	struct pollfd pfd;

	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	std::vector<struct pollfd> pfds;
	pfds.push_back(pfd);

	char buf[1024];

	/**
	 * Start polling
	 */

	signal(SIGINT, signal_handler);

	std::queue<std::pair<int, std::string> > message_queue;

	while (0xCAFE)
	{
		if (!message_queue.empty()) {
			sub_to_pollout(pfds, message_queue.front().first);
		}

		rc = poll(&pfds[0], pfds.size(), -1);

		if (rc == -1)
		{
			if (errno == EINTR)
				break ;
			perror("poll() failed");
			close(server_fd);
			return (1);
		}

		if (pfds[0].revents & POLLIN)
		{
			/**
			 * Accept
			 */

			struct sockaddr_in client_addr;
			socklen_t client_addr_len = sizeof(client_addr);
			int client_fd;
			client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
			if (client_fd == -1) {
				perror("accept() failed");
				close(server_fd);
				return (1);
			}
			LOGGER("Client " << client_fd << " connected from " << inet_ntoa(client_addr.sin_addr));
			pfd.fd = client_fd;
			pfd.events = POLLIN;
			pfd.revents = 0;
			pfds.push_back(pfd);
			continue ;
		}

		if (rc)
		{
			bool pop = false;
			int nbr = rc;
			size_t size = pfds.size();

			for (size_t i = 0; i < size; i++) {
				if (!nbr)
					break ;
				if (pfds[i].revents & POLLIN) {
					nbr--;
					memset(buf, 0, 1024);
					int rc = recv(pfds[i].fd, buf, sizeof(buf), 0);
					if (!rc)
					{
						LOGGER("Client " << pfds[i].fd << " disconnected");
						close(pfds[i].fd);
						pfds.erase(pfds.begin() + i);
						continue ;
					}
					std::string str = "Client " + std::to_string(pfds[i].fd) + " : " + std::string(buf);
					std::cout << str;
					message_queue.push(std::make_pair(i, str));
				}
				else if (pfds[i].revents & POLLOUT) {
					std::string str = message_queue.front().second;
					send(pfds[i].fd, str.c_str(), strlen(str.c_str()), 0);
					pfds[i].events &= ~POLLOUT;
					pop = true;
				}
			}

			if (pop) {
				message_queue.pop();
			}
		}
	}
	// print msg queue
	// while (!message_queue.empty())
	// {
		// std::cout << message_queue.front().second;
		// message_queue.pop();
	// }
	close(server_fd);
}

void sub_to_pollout(std::vector<struct pollfd> &pfds, size_t i)
{
	for (size_t j = 1; j < pfds.size(); j++) {
		if (i != j)
			pfds[j].events |= POLLOUT;
	}
}