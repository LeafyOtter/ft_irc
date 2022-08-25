#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	// Possible errors :
	// [ ] no password given
	// [ ] already registered

	void	Server::cmd_pass(int fd, arguments_t &args)
	{
		std::string name = "PASS";
		std::string pass;
		std::string nick;
		c_irc::User &user = *users[fd];

		nick = user.get_nick();
		nick = nick.empty() ? "*" : nick;

		LOG_USER(fd, "attempting to register");
		if (args.size() == 0)
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}

		if (user.get_mode() & U_MODE_REGISTERED_PASS)
		{
			queue_message(ERR_ALREADYREGISTERED(nick), fd);
			return ;
		}

		pass = args[0];

		if (pass != get_password()) {
			LOG_USER(fd, "wrong password");
			return ;
		}
		
		user.set_mode(user.get_mode() | U_MODE_REGISTERED_PASS);
		LOG_USER(fd, "successfully registered");
	}
}