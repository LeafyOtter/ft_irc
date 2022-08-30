#include "Server.hpp"

#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_quit(int fd, arguments_t &args)
	{
		(void)args;
		queue_message(RPL_ERROR, fd);
	}
} // namespace c_irc