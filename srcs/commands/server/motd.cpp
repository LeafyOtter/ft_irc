#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>
#include <fstream>

namespace c_irc
{
	void Server::cmd_motd(int fd, arguments_t &args)
	{
		(void)args;
		std::string msg = "";
		std::string line;
		std::string nick = users[fd]->get_nick();

		if (users[fd]->is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(users[fd]->get_nick()), fd);
			return ;
		}

		std::ifstream file(MOTD_FILE, std::ios::in);
		if (not file.is_open())
		{
			queue_message(ERR_NOMOTD(nick), fd);
			return ;
		}
		msg += RPL_MOTDSTART(nick);
		while (getline(file, line))
			msg += RPL_MOTD(nick, line);
		msg += RPL_ENDOFMOTD(nick);
		queue_message(msg, fd);
		file.close();
	}
} // namespace c_irc