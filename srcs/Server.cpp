#include "Command.hpp"
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
	{
		for (size_t i = 0; i < users.size(); i++) {
			if (users[i] != NULL)
			{
				close(users[i]->get_fd());
				delete users[i];
			}
		}
		for (channels_it_t it = channels.begin(); it != channels.end(); it++)
			delete it->second;
		pollfds.clear();
		users.clear();
		channels.clear();
		while (not buffer.empty()) {
			delete buffer.front();
			buffer.pop();
		}
		close(fd);
	}

	void	Server::initialize(	std::string new_name,
								std::string new_port,
								std::string new_password)
	{
		int rc;

		name = new_name;
		password = new_password;
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

		init_commands();
		creation_time = c_irc::get_time();
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
			if (pollfds[0].revents == POLLIN) // new connection to server waiting
				accept_connections();
			if (rc)
				check_all_clients(rc);
			if (not buffer.empty() and buffer.front()->get_status()) {
				delete buffer.front();
				buffer.pop();
			}
		}
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

		LOG_USER(pfd.fd, "User class connected");
		c_irc::User *new_user = new c_irc::User(pfd.fd, pollfds); // TODO : refactor
		users.insert(std::make_pair(pfd.fd, new_user));
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
					delete_user(i, pollfds[i].fd);
					continue ;
				}
				parse_message(buf, pollfds[i].fd);
			}
			if (pollfds[i].revents & POLLOUT)
				send_message(buffer.front(), pollfds[i]);
		}
	}

	void	Server::send_message(c_irc::Message *msg, pollfd &pfd)
	{
		std::string str = msg->get_message();
		send(pfd.fd, str.c_str(), str.length(), 0);
		pfd.events &= ~POLLOUT;
		msg->set_status();
	}

	void	Server::create_channel(std::string name, int user)
	{
		c_irc::Channel *ptr = new c_irc::Channel(users, name, user);
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
		else
			LOG("Channel " + name + " not found");
	}

	void	Server::delete_user(int index, int fd)
	{
		LOG_USER(fd, "disconnected from server");
		close(fd);
		for (channels_it_t it = channels.begin(); it != channels.end(); it++)
			(*it).second->remove_user(fd);
		serv_users_it_t it = users.find(fd);
		delete (*it).second;
		users.erase(it);
		pollfds.erase(pollfds.begin() + index);
	}

	void	Server::parse_message(std::string msg, int fd)
	{
		std::string delimiter = "\r\n";
		size_t pos = 0;

		users[fd]->append_buffer(msg);

		while (1)
		{
			msg = users[fd]->get_buffer();
			pos = msg.find(delimiter);
			if (pos == std::string::npos)
				return ;
			users[fd]->set_buffer(msg.substr(pos + delimiter.length()));

			c_irc::Command cmd(msg.substr(0, pos));
		
			if (not cmd.get_cmd().empty())
				std::cout << cmd;
			execute_command(cmd, fd);
		}
	}

	void	Server::execute_command(c_irc::Command &cmd, int fd)
	{
		commands_t::iterator it = commands.find(cmd.get_cmd());

		if (it == commands.end())
			return ;
		cmd_ptr ptr = (*it).second;
		(this->*ptr)(fd, cmd.get_args());
	}

	void	Server::init_commands()
	{
		commands["NICK"] = &Server::cmd_nick;
		commands["PASS"] = &Server::cmd_pass;
		commands["USER"] = &Server::cmd_user;
		commands["CAP"] = &Server::cmd_cap;
	}

	void Server::queue_message(std::string payload, int fd)
	{
		c_irc::Message *msg = new c_irc::Message(users, fd, payload);
		buffer.push(msg);
	}

	void Server::queue_message(std::string payload, chan_users_it_t first, chan_users_it_t last)
	{
		c_irc::Message *msg = new c_irc::Message(users, first, last);
		msg->set_message(payload);
		buffer.push(msg);
	}

	std::string Server::get_password() const { return (password);}

} // namespace c_irc