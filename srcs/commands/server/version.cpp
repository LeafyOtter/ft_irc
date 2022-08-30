#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_version(int fd, arguments_t &args)
	{
		std::string msg = "";
		std::string nick = users[fd]->get_nick();

		if (args.size() == 1 and args[0] != "c-irc.net")
		{
			queue_message(ERR_NOSUCHSERVER(nick, args[0]), fd);
			return ;
		}
		msg += RPL_VERSION(nick, "c-irc.net", VERSION);
		queue_message(msg, fd);
	}
} // namespace c_irc