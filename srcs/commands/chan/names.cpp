#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"
#include "../replies.hpp"
/*



	A FAIRE :
		- At the end of this list, a list of users who
		are visible but either not on any channel or not on a visible channel
		are listed as being on `channel' "*". 

	Numeric error and Replies:   * -> dans le fichier erreur // ** -> utilisé

	There is no error reply for bad channel names.
	ERR_TOOMANYMATCHES (a verif)			*ERR_NOSUCHSERVER
	**RPL_NAMREPLY (a faire plus tard)		**RPL_ENDOFNAMES (a faire plus tard)

*/
namespace c_irc
{
	std::string chan_state(Channel *chan)
	{
		if (chan->is_mode(C_MODE_SECRET))
			return ("@ ");			// chaine secret
		else if (chan->is_mode(C_MODE_PRIVATE))
			return ("* ");			// chaine privee
		else 
			return ("= ");			// chaine public
	}

	std::string	Server::print_users_list(Channel *chan, int fd, std::string nick)
	{
		std::set<int>	tmp;

		return (print_users_list(chan, fd, nick, tmp));
	}

	std::string	Server::print_users_list(Channel *chan, int fd, std::string nick, std::set<int> &user_in_chan)
	{
		std::string chan_name = chan_state(chan) + chan->get_name();
		std::string op_user = "";
		std::string normal_user = "";
		std::string msg;

		for (chan_users_it_t it = chan->begin(); it != chan->end(); it++)
		{
			if (users[it->first]->is_mode(U_MODE_INVISIBLE) \
				and not chan->is_user_in_channel(fd))
				continue ;

			if (it->second)
			{
				if (not op_user.empty())
					op_user += " ";
				op_user += "@" + users[it->first]->get_nick();
			}
			else
			{
				if (not normal_user.empty())
					normal_user += " ";
				normal_user += users[it->first]->get_nick();
			}
			user_in_chan.insert(it->first);
		}

		msg = op_user + " " + normal_user;
		if (op_user.empty())
			msg = normal_user;
		if (normal_user.empty())
			msg = op_user;
		msg = RPL_NAMREPLY(nick, chan_name, msg);
		msg += RPL_ENDOFNAMES(nick, chan->get_name());
		return (msg);
	}

	void Server::cmd_names(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::map<std::string, c_irc::Channel *>::iterator it; 
		std::string nick;
		std::string msg = "";
		std::set<int> user_in_chan;
		std::string tmp;
		nick = user.get_nick();

		if (user.is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(nick), fd);
			return ;
		}

		if (args.size() == 0)
		{	
			for (it = channels.begin(); it != channels.end(); it++ ) 
				if (!(it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick))
					msg += print_users_list(it->second, fd, nick, user_in_chan);
			
			for (serv_users_it_t it = users.begin(); it != users.end(); it++)
			{
				if (it->second->is_mode(U_MODE_INVISIBLE) or user_in_chan.count(it->first))
					continue ;
				if (not tmp.empty())
					tmp += " ";
				tmp += it->second->get_nick();
			}
			msg += RPL_NAMREPLY(nick, "*", tmp);
			msg += RPL_ENDOFNAMES(nick, "*");
			queue_message(msg, fd);
			return ;
		}

		std::vector <std::string> element;
			split(args[0], ',', element); 
		for(size_t i = 0; i < element.size(); i++)
		{
			if ((it = channels.find(element[i])) != channels.end())
			{
				if (!(it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick))	// si ce n'est pas un channel secret ou que l'user et dedans ok
					msg += print_users_list(it->second, fd, nick);
				else	// sinon envoyer un endofnames
					msg += RPL_ENDOFNAMES(nick, it->second->get_name());
			}
			else
				msg += RPL_ENDOFNAMES(nick, element[i]); // envoyer aussi un endofname si channel inexistant
		}
		queue_message(msg, fd);
	}
} 
