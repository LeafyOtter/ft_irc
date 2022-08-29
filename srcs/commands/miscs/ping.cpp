#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void	Server::cmd_ping(int fd, arguments_t &args)
	{
		LOG_USER(fd, "sent PING command. Sending PONG.");
		if (args.size() < 0)
		{
			queue_message(ERR_NOORIGIN(users[fd]->get_nick()), fd);
			return ;
		}
		queue_message(RPL_PONG, fd);
	}
} // namespace c_irc
