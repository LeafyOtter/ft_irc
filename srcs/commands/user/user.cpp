#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"

#include "utils.tpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_user(int fd, arguments_t &args)
	{
		std::string nick;
		c_irc::User &user = *users[fd];

		nick = user.get_nick();
		if (args.size() < 4)
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, "USER"), fd);
			return ;
		}

		if (args.size() > 4)
			return ;

		if (user.get_mode() & U_MODE_REGISTERED_USER)
		{
			queue_message(ERR_ALREADYREGISTERED(nick), fd);
			return ;
		}

		user.set_user(args[0]);
		LOG_USER(fd, "set user to " << args[0]);

		if (c_irc::stoi(args[1]) & 0x0002)
		{
			user.set_mode(user.get_mode() | U_MODE_INVISIBLE);
			LOG_USER(fd, "set mode to invisible");
		}

		// args[2] : unused

		user.set_realname(args[3]);
		LOG_USER(fd, "set realname to " << args[3]);

		user.set_mode(user.get_mode() | U_MODE_REGISTERED_USER);

		if (user.get_mode() & U_MODE_RESTRICTED)
			welcome(fd);
	}
} // namespace c_irc
