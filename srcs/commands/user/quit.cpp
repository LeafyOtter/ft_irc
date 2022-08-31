#include "Server.hpp"

#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_quit(int fd, arguments_t &args)
	{
		if (args.size() == 1)
			users[fd]->set_leave_msg(args[0]);
		users[fd]->set_delete();
		queue_message(RPL_ERROR, fd);
	}
} // namespace c_irc