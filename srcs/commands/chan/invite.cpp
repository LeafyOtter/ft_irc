#include "Server.hpp"
#include <iostream>
#include "../errors.hpp"
#include "../replies.hpp"

/*
   
   Parameters: <nickname> <channel>

   A FAIRE : (tout)
   
   The INVITE command is used to invite a user to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  However, if the channel exists, only members of the channel
   are allowed to invite other users.  When the channel has invite-only
   flag set, only channel operators may issue INVITE command.

   Only the user inviting and the user being invited will receive
   notification of the invitation.  Other channel members are not
   notified.  (This is unlike the MODE changes, and is occasionally the
   source of trouble for users.)

A VERIF :
        - NOSUCHCHANNEL n'est pas dans les erreurs alors que le canal doit forcement exister pour pouvoir inviter un user
   Numeric Replies:

           **ERR_NEEDMOREPARAMS             **ERR_NOSUCHNICK
           *ERR_NOTONCHANNEL                **ERR_USERONCHANNEL
           **ERR_CHANOPRIVSNEEDED
           **RPL_INVITING                   *RPL_AWAY (a voir si on utilise)

   Examples:

   :Angel!wings@irc.org INVITE Wiz #Dust

                                   ; Message to WiZ when he has been
                                   invited by user Angel to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone

*/

namespace c_irc
{
	void Server::cmd_invite(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
                std::string name = "INVITE";
                std::string nick = user.get_nick();
                //int pos; 
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
                if (args.size() < 1)
                        return;
                //if (args[0] != utilisateur existant)                                          // invité inconnu 
                //{
                //        queue_message(ERR_NOSUCHNICK(args[0], "nick"), fd); 
                //}     
                //  if (user mode = absent)                                                     // a verif, ou recuperer l'info 
                //         RPL_AWAY
                //if (!(channels.find(args[1]) != channels.end()))                              // channel non trouvé --> pas d'erreur ? 
                //        return; 
                //if (channels[args[1]]->is_user_in_channel(args[0]))                           // invité deja dans le channel 
                //{
                //        queue_message(ERR_USERONCHANNEL(args[0], args[1]), fd); 
                //        return; 
                //}
                //if (user n'est pas dans le channel)                                           // utilisateur n'est pas dans le channel
                //{
                //        queue_message(ERR_NOTONCHANNEL(args[1]), fd); 
                //}
                //if (user n'a pas les droit)                                                   // utilisateur n'yant pas les droit d'inviter 
                //{
                //        queue_message(CHANOPROVSNEEDED(args[1]), fd); 
                //        return
                //}
                queue_message(RPL_INVITING(args[0], args[1]), fd); 
	}
} 