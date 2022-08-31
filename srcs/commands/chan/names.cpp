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
			return ("@");			   // chaine secret
		else if (chan->is_mode(C_MODE_PRIVATE))
			return ("*");			    // chaine privee
		else 
			return ("=");			   // chaine public
	}
	std::string highest_mode_user(User *user)
	{
		// voir le mode de l'user sur le channel etc...
		//retourner le plus elevé
		// retourner si invisible
		(void)user;
		return ("");
	}
	void Server::print_users_list(Channel *chan, int fd, std::string nick)
	{
		std::string chan_type = chan_state(chan);  
		std::string user_chan_type; 
		std::string chan_name = chan->get_name();
		std::string user_chan;
		
		std::map<int, c_irc::User *>::iterator it; 

		// iterer sur les user du channel en parametre. 
			// recuperer son type dans nick_type avec la fct highest_mode_user(User *user)
			// si invisible et user n'est pas dans le channel l'affiche pas 
			// sinon
			// queue_message(RPL_NAMREPLY(user_chan_type, user_chan, chan_type, chan_name), fd); 
						// voir pour une faire boucle sur la list d'user... nouveau queue_message ? // user separe par des ','


		// rien de special sur un chan privé
 
		// a envoyer a la fin de la list
		queue_message(RPL_ENDOFNAMES(nick, chan_name), fd);
	}      

	void Server::cmd_names(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::map<std::string, c_irc::Channel *>::iterator it; 
		std::string nick;
		nick = user.get_nick();
		if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
		if (args.size() == 0)
		{	
			for (it = channels.begin(); it != channels.end(); it++ ) 
				if (!(it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick))
					print_users_list(it->second, fd, nick);
		}
		if (args.size() > 0)
		{
			std::vector <std::string> element;
				split(args[0], ',', element); 
			for(size_t i = 0; i < element.size(); i++)
			{
				if ((it = channels.find(element[i])) != channels.end())
				{
					if (!(it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick))	// si ce n'est pas un channel secret ou que l'user et dedans ok
						print_users_list(it->second, fd, nick);
					else	// sinon envoyer un endofnames
						queue_message(RPL_ENDOFNAMES(nick, it->second->get_name()), fd);
				}
				else
					queue_message(RPL_ENDOFNAMES(nick, element[i]), fd);	// envoyer aussi un endofname si channel inexistant
			}
		}
	}
} 
