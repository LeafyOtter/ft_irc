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
		close(server_fd);
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

		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd == -1)
			throw std::runtime_error("socket() failed to open");

		LOG("Socket created");

		fcntl(server_fd, F_SETFL, O_NONBLOCK);

		/*
		 *	Socket binding
		 */

		struct sockaddr_in server_addr;

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = port;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		rc = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (rc < 0) {
			close(server_fd);
			throw std::runtime_error("bind() failed to open");
		}

		LOG("Socket binded");

		/*
		 *	Listening socket
		 */

		rc = listen(server_fd, 42);
		if (rc < 0) {
			close(server_fd);
			throw std::runtime_error("listen() failed to open");
		}

		pollfd pfd;

		pfd.fd = server_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		pollfds.push_back(pfd);

		LOG("listen() success");
		LOG("Address : " << inet_ntoa(server_addr.sin_addr));
		LOG("Listening on port: " << ntohs(server_addr.sin_port));

		init_commands();
		creation_time = c_irc::get_time();
		compilation_time = __TIMESTAMP__;
	}

	void	Server::start()
	{
		int rc;

		// Change signal behavior
		signal(SIGINT, &signal_handler);
		signal(SIGTERM, &signal_handler);
		signal(SIGQUIT, &signal_handler);
		pollfds.reserve(42);
		while (0xCAFE) {
			if (not buffer.empty())
				buffer.front()->prepare();
			rc = poll(&pollfds[0], pollfds.size(), 2500);
			if (rc == -1) {
				if (errno == EINTR)
					break ;
				close(server_fd);
				throw std::runtime_error("poll() error : " + std::string(strerror(errno)));
			}
			if (rc == 0) {
				// LOG("poll() timeout");
				check_user_quit();
				continue ;
			}
			if (pollfds[0].revents == POLLIN) // new connection to server waiting
				accept_connections();
			if (rc)
				check_all_clients(rc);

			while (not buffer.front()->nb_targets()) {
				delete buffer.front();
				buffer.pop();
				if (buffer.empty())
					break ;
			}

			if (not buffer.empty() and buffer.front()->get_status())
			{
				delete buffer.front();
				buffer.pop();
				delete_empty_channels();
			}
		}
	}

	void	Server::accept_connections()
	{
		struct sockaddr_in user_addr;
		socklen_t user_addr_len = sizeof(user_addr);
		int user_fd;
		user_fd = accept(server_fd, (struct sockaddr *)&user_addr, &user_addr_len);
		if (user_fd == -1) {
			close(server_fd);
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
			if (pollfds[i].revents & POLLOUT)
			{
				send_message(buffer.front(), pollfds[i]);
				check_user_quit();
			}
			if (pollfds[i].revents & POLLIN) {
				std::fill(buf, buf + sizeof(buf), 0);
				int rc = recv(pollfds[i].fd, buf, sizeof(buf) - 1, 0);
				if (!rc) {
					delete_user(i, pollfds[i].fd);
					continue ;
				}
				parse_message(buf, pollfds[i].fd);
			}

		}
	}

	void	Server::send_message(c_irc::Message *msg, pollfd &pfd)
	{
		std::cout << "Sending message to " << pfd.fd << " : ";
		std::cout << COLOR_CYAN << msg->get_message() << COLOR_RESET;
		std::string str = msg->get_message();
		send(pfd.fd, str.c_str(), str.length(), MSG_NOSIGNAL);
		pfd.events &= ~POLLOUT;
		msg->set_status();
	}

	void	Server::create_channel(std::string name, int user, std::string key)
	{
		if (channels.find(name) != channels.end()) {
			channels[name]->add_user(user);
			LOG("User " << user << " added to channel " << name);
			return ;
		}
		c_irc::Channel *ptr = new c_irc::Channel(users, name, user);
		channels.insert(std::make_pair(name, ptr));
		if (key.empty())
			{LOG("Channel " + name + " created");}
		else 
			LOG("Channel " + name + " created with key"); 
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

	void	Server::delete_user(int fd)
	{
		for (size_t i = 0; i < pollfds.size(); i++) {
			if (pollfds[i].fd == fd) {
				delete_user(i, fd);
				return ;
			}
		}
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
		pollfds[index] = (pollfd){0, 0, 0};
	}

	void	Server::parse_message(std::string msg, int fd)
	{
		std::string delimiter = "\r\n";
		size_t pos = 0;

		if (msg.empty())
			return ;

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
			if (users.find(fd) == users.end())
				return ;
		}
	}

	void	Server::execute_command(c_irc::Command &cmd, int fd)
	{
		commands_t::iterator it = commands.find(cmd.get_cmd());

		if (it == commands.end())
			return ;
		LOG_USER(fd, "Executing command : " + cmd.get_cmd());
		cmd_ptr ptr = (*it).second;
		(this->*ptr)(fd, cmd.get_args());
	}

	void	Server::init_commands()
	{
		commands["NICK"] = &Server::cmd_nick;
		commands["PASS"] = &Server::cmd_pass;
		commands["USER"] = &Server::cmd_user;
		commands["CAP"] = &Server::cmd_cap;
		commands["JOIN"] = &Server::cmd_join; 
		commands["INVITE"] = &Server::cmd_invite; 
		commands["KICK"] = &Server::cmd_kick; 
		commands["LIST"] = &Server::cmd_list; 
		commands["NAMES"] = &Server::cmd_names; 
		commands["PART"] = &Server::cmd_part; 
		commands["TOPIC"] = &Server::cmd_topic; 

		commands["MODE"] = &Server::cmd_mode;
		commands["PING"] = &Server::cmd_ping;
		commands["TIME"] = &Server::cmd_time;
		commands["OPER"] = &Server::cmd_oper;

		commands["PRIVMSG"] = &Server::cmd_privmsg;
		commands["NOTICE"] = &Server::cmd_notice;
		commands["QUIT"] = &Server::cmd_quit;
		commands["KILL"] = &Server::cmd_kill;

		commands["INFO"] = &Server::cmd_info;
		commands["MOTD"] = &Server::cmd_motd;
		commands["VERSION"] = &Server::cmd_version;
	}

	void Server::queue_message(std::string payload, int fd)
	{
		c_irc::Message *msg;

		msg = new c_irc::Message(users, payload, fd);
		buffer.push(msg);
	}

	void Server::queue_message(std::string payload, c_irc::Channel *chan)
	{
		c_irc::Message *msg;

		msg = new c_irc::Message(users, payload, chan, 0);
		buffer.push(msg);
	}

	void Server::queue_message(std::string payload, c_irc::Channel *chan, int fd)
	{
		c_irc::Message *msg;

		msg = new c_irc::Message(users, payload, chan, fd);
		buffer.push(msg);
	}

	std::string Server::get_password() const { return (password);}

	int Server::is_user(std::string name)
	{
		for (serv_users_it_t it = users.begin(); it != users.end(); it++)
			if ((*it).second->get_nick() == name)
				return ((*it).first);
		return (-1);
	}

	void Server::check_user_quit()
	{
		serv_users_it_t next;

		for (serv_users_it_t it = users.begin(); it != users.end(); it = next)
		{
			next = it;
			next++;
			if (it->second->is_delete())
				delete_user(it->first);
		}
	}

	void Server::delete_empty_channels()
	{
		channels_it_t next;

		for (channels_it_t it = channels.begin(); it != channels.end(); it = next)
		{
			next = it;
			next++;

			if ((*it).second->is_empty())
				delete_channel((*it).first);
		}
	}
} // namespace c_irc