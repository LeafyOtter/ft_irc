#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"
#include <string.h>
#include <iostream>

/*

   Numeric error and Replies:	* -> dans le fichier erreur // ** -> utilisé

	**ERR_NEEDMOREPARAMS	**ERR_NOTONCHANNEL
	**RPL_NOTOPIC			**RPL_TOPIC
	**ERR_CHANOPRIVSNEEDED	ERR_NOCHANMODES (a verif) (non)

	A FAIRE :
		- verif l'erreur nochanmode
		- verifier le droit user avant de modifier le topic
		- reduir la fonctions ?
	A VERIFIER : 
		- si le topic est vide de base, peut on quand meme le modifier -> If the <topic> parameter is present, the topic for that
		channel will be changed + If the <topic> parameter is an empty string, the topic for that channel will be removed. (?? bool +  string pour topic)
		
		- si le channel n'existe pas on fait rien ou on envoi que l'user n'est pas sur le channel ? NOSUCHECHANNEL n'est pas une erreur de TOPIC

*/

namespace c_irc
{
	void Server::cmd_topic(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::string name = "TOPIC";
		std::string nick = user.get_nick();
		std::string msg;
		c_irc::Channel *chan;

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

		if (channels.find(args[0]) == channels.end())
		{
			queue_message(ERR_NOSUCHCHANNEL(nick, args[0]), fd);
			return ;
		}

		chan = channels[args[0]];

		if (chan->is_mode(C_MODE_PRIVATE) \
			and not chan->is_user_in_channel(nick))
		{
			queue_message(ERR_NOTONCHANNEL(nick, args[0]), fd);
			return ;
		}

		if (args.size() == 1)	// one arg, just send back topic
		{
			if (chan->get_topic().empty())
				msg += RPL_NOTOPIC(nick, args[0]);
			else
				msg += RPL_TOPIC(nick, args[0], chan->get_topic());
			queue_message(msg, fd);
			return ;
		}

		if (not chan->is_user_in_channel(nick))
		{
			queue_message(ERR_NOTONCHANNEL(nick, args[0]), fd);
			return ;
		}

		if (chan->is_mode(C_MODE_TOPIC_LOCK) \
			and not chan->is_user_op(fd))
		{
			queue_message(ERR_CHANOPRIVSNEEDED(nick, args[0]), fd);
			return ;
		}

		chan->set_topic(args[1]);
		queue_message(RPL_TOPIC_CHANGE(nick, user.get_user(), chan->get_name(), args[1]), chan);
	}
}