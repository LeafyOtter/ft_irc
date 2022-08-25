#include "Server.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_cap(int fd, arguments_t &args)
	{
		(void)args;
		LOG_USER(fd, "sent CAP command. Ignoring.");
	}
} // namespace c_irc