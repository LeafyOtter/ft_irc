#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"
#include "../replies.hpp"

/*

Command: KICK
Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
[<comment>]

The KICK command can be used to request the forced removal of a user
from a channel.  It causes the <user> to PART from the <channel> by
force.  For the message to be syntactically correct, there MUST be
either one channel parameter and multiple user parameter, or as many
channel parameters as there are user parameters.  If a "comment" is
given, this will be sent instead of the default message, the nickname
of the user issuing the KICK.

The server MUST NOT send KICK messages with multiple channels or
users to clients.  This is necessarily to maintain backward
compatibility with old client software.

Numeric Replies:

 **ERR_NEEDMOREPARAMS					*ERR_NOSUCHCHANNEL
 *ERR_BADCHANMASK (noté obsolete...)	**ERR_CHANOPRIVSNEEDED
 **ERR_USERNOTINCHANNEL					*ERR_NOTONCHANNEL
 Renvoyé lorsqu'un client essaie d'exécuter une commande
 affectant le canal sur un canal dont le client ne fait pas partie
 A VERIF :

 - pkoi utiliser BADCANMASK pour un nom invalid au lieu d'utiliser NOSUCHCHANNEL pour dire qu'il n'existe pas (quand utiliser l'un ou l'autre ?)
 - Les serveurs PEUVENT limiter le nombre d'utilisateurs cibles par KICKcommande 
 via le TARGMAX paramètre de RPL_ISUPPORT , et supprimer silencieusement des cibles si le nombre de cibles dépasse la limite.

 ok verifié : 1 chan et plusieurs user, ou un chan par user, mais pas plusieurs urser sur un seul chan : 
 there MUST be either one channel parameter and multiple user parameter, or as many channel parameters as there are user parameters. 

*/

namespace c_irc
{

	void Server::cmd_kick(int fd, arguments_t &args)
	{ 
		c_irc::User &user = *users[fd];
		std::string name = "KICK";
		std::string nick = user.get_nick();
		channels_t::iterator it;
		std::string comment = "";
		std::vector <std::string> users_to_kick;
		std::vector <std::string> from_chan_kick;
		std::string chan_name; 
		

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
		if (args.size() == 1)	// pas d'erreur si il n'y a aucun user entré
			return; 
		if (args.size() == 3)
				comment = args[2];
		split(args[0], ',', from_chan_kick); 
		split(args[1], ',', users_to_kick);

		if (from_chan_kick.size() > users_to_kick.size())
		{
			LOG_USER(fd, "Error, too many channels");
			return ;
		}
		if (from_chan_kick.size() == 1)		// si le vecteur de channel = 1, virer tous les utilisateur du vecteur d'user
		{
			chan_name = from_chan_kick[0]; 
			if (not (channels.find(chan_name) != channels.end()))	// si le channel n'existe pas -> erreur
			{
				queue_message(ERR_NOSUCHCHANNEL(nick, chan_name), fd);
				return; 
			}
			if (not channels[chan_name]->is_user_op(fd))	// si l'user n'est pas operateur sur ce channel -> return; 
			{
				queue_message(ERR_CHANOPRIVSNEEDED(nick, args[1]), fd);
				return ;
			}  
			for(size_t i = 0; i < users_to_kick.size(); i++)	// iteration sur les users donné en param 
			{
				if (not channels[chan_name]->is_user_in_channel(users_to_kick[i]))	// verif si ils sont bien dans le channel
					queue_message(ERR_USERNOTINCHANNEL(nick, users_to_kick[i], chan_name), fd);
				else 
				{
					channels[chan_name]->remove_user(users_to_kick[i]); 
					queue_message(RPL_PART(nick, users_to_kick[i], chan_name, comment), fd);	 // message de sortie
				}
			}
		}
		else if (from_chan_kick.size() == users_to_kick.size()) // si il y a autant de chanel que d'user
		{
			for(size_t i = 0; i < from_chan_kick.size(); i++) 
			{
				if (not (channels.find(from_chan_kick[i]) != channels.end()))
				{
					queue_message(ERR_NOSUCHCHANNEL(nick, from_chan_kick[i]), fd);
					return; 
				} 
				if (not channels[from_chan_kick[i]]->is_user_op(fd))
				{
					queue_message(ERR_CHANOPRIVSNEEDED(nick, args[1]), fd);
					return ;
				}  
				if (not channels[from_chan_kick[i]]->is_user_in_channel(users_to_kick[i]))
					queue_message(ERR_USERNOTINCHANNEL(nick, users_to_kick[i], from_chan_kick[i]), fd);
				else
				{
					channels[from_chan_kick[i]]->remove_user(users_to_kick[i]);
					queue_message(RPL_PART(nick, users_to_kick[i], from_chan_kick[i], comment), fd);
				}
			}
		}

	}
}
