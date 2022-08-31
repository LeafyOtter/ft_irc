#include "Server.hpp"

#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_cap(int fd, arguments_t &args)
	{
		(void)args;

		if (args.size() >= 1 and args[0] == "LS")
		{
			LOG_USER(fd, "sent CAP command. Sent back empty list.");
			queue_message(RPL_CAP, fd);
			return ;
		}
		else
			LOG_USER(fd, "sent CAP command. Ignoring.");
	}
} // namespace c_irc