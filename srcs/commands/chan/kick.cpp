#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"

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

           **ERR_NEEDMOREPARAMS                 *ERR_NOSUCHCHANNEL
           *ERR_BADCHANMASK (noté obsolete...)  **ERR_CHANOPRIVSNEEDED
           **ERR_USERNOTINCHANNEL               *ERR_NOTONCHANNEL =  
                                                Renvoyé lorsqu'un client essaie d'exécuter une commande 
                                                affectant le canal sur un canal dont le client ne fait pas partie.

   
        A FAIRE : TOUT
  
        A VERIF :
                - peut on virer une personne de plusieurs chan ? 
                - peut on virer plusieurs personne d'un chan ? 
                - peut on virer plusieurs personne de plusieurs chan ? 
                - pkoi utiliser BADCANMASK pour un nom invalid au lieu d'utiliser NOSUCHCHANNEL pour dire qu'il n'existe pas (quand utiliser l'un ou l'autre ?)
                - revoir NOTONCHANNEL 
                - Les serveurs PEUVENT limiter le nombre d'utilisateurs cibles par KICKcommande 
                via le TARGMAXparamètre deRPL_ISUPPORT , et supprimer silencieusement des cibles si le nombre de cibles dépasse la limite.
*/

namespace c_irc
{
	void Server::cmd_kick(int fd, arguments_t &args)
	{ 
                c_irc::User &user = *users[fd];
                std::string name = "KICK";
                std::string nick = user.get_nick();
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
                
                // verifier que l'user a le droit de sortir ERR_CHANOPRIVSNEEDED + si il est sur le channel concerné ? ERR_NOTONCHANNEL
                // prend l'args 0, le split et verifier que les channels existent sinon nosuchchannel ?
                // si oui -> virer les args 1, 1 par un et verifier si il ils etaient bien sur le channel sinon ERR_USERNOTINCHANNEL
                // regarde si il y a dautre channel dans vector et banuser()
                // si argv2 --> envoyer un message avec la raison 
	}
} 