#include "Server.hpp"
#include "Message.hpp"

#include "utils.tpp"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
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
		port = htons(c_irc::stoi(new_port));	// replace stoi (not std=c++98 compliant)

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
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
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
		LOG("Address : " << inet_ntoa(server_addr.sin_addr));
		LOG("Listening on port: " << ntohs(server_addr.sin_port));
	}

	void	Server::start()
	{
		int rc;

		// Change signal behavior
		signal(SIGINT, &signal_handler);
		// signal(SIGTERM, &signal_handler);
		// signal(SIGQUIT, &signal_handler);

		pollfds.reserve(42);

		while (0xCAFE) {

			if (not buffer.empty()) {
				if (not buffer.front()->nb_users()) {
					delete buffer.front();
					buffer.pop();
				}
				else
					buffer.front()->prepare();
			}

			rc = poll(&pollfds[0], pollfds.size(), -1);

			if (rc == -1) {
				if (errno == EINTR)
					break ;
				close(fd);
				throw std::runtime_error("poll() error : " + std::string(strerror(errno)));
			}

			if (pollfds[0].revents == POLLIN) { // new connection to server waiting
				accept_connections();
				// continue ;
			}

			if (rc) {
				check_all_clients(rc);
			}

			if (not buffer.empty() and buffer.front()->get_status()) {
				delete buffer.front();
				buffer.pop();
			}
			// LOG("users : " << users.size());
		}

		close(fd);
		for (users_it_t it = users.begin(); it != users.end(); ++it) {
			close((*it).first->get_pfd()->fd);
			delete (*it).first;
		}
		users.clear();
		while (not buffer.empty()) {
			delete buffer.front();
			buffer.pop();
		}

		delete_channel("#test");
	}

	void	Server::accept_connections()
	{
		struct sockaddr_in user_addr;
		socklen_t user_addr_len = sizeof(user_addr);
		int user_fd;
		user_fd = accept(fd, (struct sockaddr *)&user_addr, &user_addr_len);
		if (user_fd == -1) {
			close(fd);
			throw std::runtime_error("accept() failed to open");
		}
		LOG("New connection from " << inet_ntoa(user_addr.sin_addr) \
			<< ":" << ntohs(user_addr.sin_port));
		pollfd pfd;
		pfd.fd = user_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		pollfds.push_back(pfd);

		LOG("New user added");
		c_irc::User *new_user = new c_irc::User(&pollfds.back()); // TODO : refactor
		new_user->set_nick(c_irc::to_string(pollfds.back().fd - 3));
		users.insert(std::make_pair(new_user, int()));
	}

	users_it_t	Server::find_user(int fd)
	{
		users_it_t it = users.begin();
		while (it != users.end()) {
			if ((*it).first->get_pfd()->fd == fd)
				return it;
			++it;
		}
		return users.end();
	}

	void	Server::check_all_clients(int n)
	{
		char	buf[1024];
		size_t	size = pollfds.size();

		for (size_t i = 1; i < size; i++) {
			if (!n)
				break ;
			if (pollfds[i].revents)
				n--;
			if (pollfds[i].revents & POLLIN) {
				std::fill(buf, buf + sizeof(buf), 0);
				int rc = recv(pollfds[i].fd, buf, sizeof(buf), 0);
				if (!rc) {
					LOG("Client " << pollfds[i].fd - 3 << " disconnected");

					close(pollfds[i].fd);
					LOG("Content of pollfd : " << pollfds[i].fd << " " << pollfds[i].events << " " << pollfds[i].revents);
					LOG("Size of users : " << users.size());

					users_it_t it = find_user(pollfds[i].fd);
					delete (*it).first;
					users.erase(it);

					memset(&pollfds[i], 0, sizeof(pollfds[i]));
					continue ;
				}

				// TODO: refactor

				c_irc::User *user = (*find_user(pollfds[i].fd)).first;

				std::string str = "Client " + c_irc::to_string(pollfds[i].fd - 3) + ": " + std::string(buf);
				std::cout << str;

				// parse message
				if (std::string(buf) == "Create Channel\n") {
					create_channel("#test", user);
					return ;
				}

				if (std::string(buf) == "Join Channel\n") {
					channels["#test"]->add_user(user);
					return ;
				}



				c_irc::Message *msg;

				// create new Message

				if (!channels.empty() && \
					channels["#test"]->is_user_in_channel(user->get_nick()))
				{
					msg = new c_irc::Message(channels["#test"]->begin(), channels["#test"]->end());
					str = "Sent from channel #test : " + str;
					msg->set_message(str);
				}
				else
				{
					msg = new c_irc::Message(users.begin(), users.end());
					msg->set_message(str);
				}

				msg->set_sender(find_user(pollfds[i].fd));

				buffer.push(msg);
			}


			if (pollfds[i].revents & POLLOUT) {
				// send_message(buffer.front(), pollfds[i]);
				std::string str = buffer.front()->get_message();
				send(pollfds[i].fd, str.c_str(), str.length(), 0);
				pollfds[i].events &= ~POLLOUT;
				buffer.front()->set_status();
			}
		}
	}

	void	Server::send_message(c_irc::Message *msg, pollfd &pfd)
	{
		std::string str = msg->get_message();
		send(pfd.fd, str.c_str(), str.length(), 0);
		pfd.fd &= ~POLLOUT;
		msg->set_status();
	}

	void	Server::create_channel(std::string name, c_irc::User *user)
	{
		c_irc::Channel *ptr = new c_irc::Channel(name, user);
		channels.insert(std::make_pair(name, ptr));

		LOG("Channel " + name + " created");
	}

	void	Server::delete_channel(std::string name)
	{
		channels_it_t it = channels.find(name);
		if (it != channels.end()) {
			delete (*it).second;
			channels.erase(it);
			LOG("Channel " + name + " deleted");
		}
	}

} // namespace c_irc
