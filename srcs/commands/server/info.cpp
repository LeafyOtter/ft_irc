#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_info(int fd, arguments_t &args)
	{
		std::string msg = "";
		std::string nick = users[fd]->get_nick();

		if (users[fd]->is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(users[fd]->get_nick()), fd);
			return ;
		}

		if (args.size() == 1 and args[0] != "c-irc.net")
		{
			queue_message(ERR_NOSUCHSERVER(nick, args[0]), fd);
			return ;
		}
		msg += RPL_INFO(nick, "----------------- c-irc.net -----------------");
		msg += RPL_INFO(nick, " c_irc : version " + VERSION);
		msg += RPL_INFO(nick, " - : author : " + AUTHOR);
		msg += RPL_INFO(nick, " - : github : " + GITHUB);
		msg += RPL_INFO(nick, " - : compilation time : " + compilation_time);
		msg += RPL_INFO(nick, " - : start time : " + creation_time);
		msg += RPL_INFO(nick, "     A 42 school project");
		msg += RPL_INFO(nick, "---------------------------------------------");
		msg += RPL_ENDOFINFO(nick);
		queue_message(msg, fd);
	}
} // namespace c_irc