#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"
#include "../replies.hpp"

/*
   Command: PART
   Parameters: <channel> *( "," <channel> ) [ <Part Message> ]

	A FAIRE :

		- voir si on bouge la sortie avec message de replies.hpp ->   If a "Part Message" is given, this will be sent
		instead of the default message, the nickname.  This request is always granted by the server. (message dans la RFC 2851 mais pas dans la 1459)
		voir si mettre le message de l'exemple. 

	Numeric error and Replies:	* -> dans le fichier erreur // ** -> utilisé

	**ERR_NEEDMOREPARAMS	**ERR_NOSUCHCHANNEL
	**ERR_NOTONCHANNEL

	A VERIFIER :
		- :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost; User WiZ leaving channel "#playzone" with the message "I lost".
		- Servers MUST be able to parse arguments in the form of a list of target, but SHOULD NOT use lists when sending PART messages to clients.   
*/

namespace c_irc
{
	void Server::cmd_part(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::string msg;
		std::string nick = user.get_nick();
		std::string reason = ""; 
		c_irc::Channel *channel;
		size_t pos = 0;

		if (user.is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(nick), fd);
			return ;
		}

		if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}

		if (args.size() >= 2)
			reason = args[1];

		while (pos != std::string::npos)
		{
			pos = args[0].find(",");
			std::string chan_name = args[0].substr(0, pos);
			args[0] = pos != std::string::npos ? args[0].substr(pos + 1) : "";

			if (channels.find(chan_name) == channels.end())
			{
				queue_message(ERR_NOSUCHCHANNEL(nick, chan_name), fd);
				continue ;
			}

			channel = channels[chan_name];

			if (not channel->is_user_in_channel(nick))
			{
				queue_message(ERR_NOTONCHANNEL(nick, chan_name), fd);
				continue ;
			}
			channel->remove_user(fd);
			std::string tmp = RPL_PART(nick, user.get_user(), chan_name, reason);
			queue_message(tmp, channel->begin(), channel->end());
			msg += tmp;
		}
		queue_message(msg, fd);
	}
} // namespace c_irc