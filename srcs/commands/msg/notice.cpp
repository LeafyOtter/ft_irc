#include "Server.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_notice(int fd, arguments_t &args)
	{
		c_irc::User *user = users[fd];
		std::string cmd = "NOTICE";
		std::string nick = user->get_nick();
		std::string	chan_identifiers = "&#+!";

		if (user->is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(user->get_nick()), fd);
			return ;
		}


		if (args.empty())
		{
			// queue_message(ERR_NORECIPIENT(nick, cmd), fd);
			return ;
		}

		if (args[1].empty())
		{
			// queue_message(ERR_NOTEXTTOSEND(nick), fd);
			return ;
		}

		if (chan_identifiers.find_first_of(args[0][0]) != std::string::npos)
		{
			std::string name = args[0];
			c_irc::Channel *chan;
			channels_it_t it = channels.find(name);
			
			if (it == channels.end())
			{
				// queue_message(ERR_NOSUCHCHANNEL(nick, name), fd);
				return ;
			}

			chan = it->second;
			if (chan->is_mode(C_MODE_NO_EXTERNAL))
			{
				// queue_message(ERR_CANNOTSENDTOCHAN(nick, name), fd);
				return ;
			}

			std::string msg = RPL_NOTICE(nick, user->get_user(), name, args[1]);
			queue_message(msg, chan, fd);
			return ;
		}
		else
		{
			std::string name = args[0];
			int fd_target = is_user(name);

			if (fd_target == -1)
			{
				// queue_message(ERR_NOSUCHNICK(nick, name), fd);
				return ;
			}
			
			std::string msg = RPL_NOTICE(nick, user->get_user(), name, args[1]);
			queue_message(msg, fd_target);
		}
	}
} // namespace c_irc
