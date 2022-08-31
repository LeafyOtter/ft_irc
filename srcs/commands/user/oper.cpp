#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

#define LOG(x) std::cout << x << std::endl;

namespace c_irc
{
	static
	std::string umode_string(uint16_t mode)
	{
		std::string ret = "+";

		if (mode & U_MODE_INVISIBLE)
			ret += "i";
		if (mode & U_MODE_OPERATOR)
			ret += "o";
		if (mode & U_MODE_RESTRICTED)
			ret += "r";
		return (ret);
	}

	void Server::cmd_oper(int fd, arguments_t &args)
	{
		c_irc::User *user = users[fd];
		std::string msg = "";

		if (user->is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(user->get_nick()), fd);
			return ;
		}

		if (args.size() < 2)
		{
			queue_message(ERR_NEEDMOREPARAMS(user->get_nick(), "OPER"), fd);
			return ;
		}

		if (args[0] != OPER_NAME or args[1] != OPER_PASS)
		{
			queue_message(ERR_PASSWDMISMATCH(user->get_nick()), fd);
			return ;
		}

		user->set_flag_mode(U_MODE_OPERATOR);
		msg += RPL_UMODEIS(user->get_nick(), umode_string(user->get_mode()));
		msg += RPL_YOUREOPER(user->get_nick());
		queue_message(msg, fd);
	}
} // namespace c_irc
