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
        - utilisation de NOTONCHANNEL (seul les membre peuvent inviter) // diff avec CHANOPRIVSNEEDED ? (si mode -i activé et operateur non membre de chan ?)
        - NOSUCHCHANNEL n'est pas dans les erreurs alors que le canal doit forcement exister pour pouvoir inviter un user
        - mode +i 
   Numeric Replies:

           **ERR_NEEDMOREPARAMS             **ERR_NOSUCHNICK
           *ERR_NOTONCHANNEL                **ERR_USERONCHANNEL
           **ERR_CHANOPRIVSNEEDED
           **RPL_INVITING                   **RPL_AWAY

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
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
                //args[0] --> qui / user_name /  verif si exist NOSUCHNICK / verif si utilisateur absent (RPL_AWAY)
                //args[1] --> quel chan / find / verif si user deja sur channel USERONCHANNEL // verif si le droit d'inviter ERR_CHANOPRIVSNEEDED

                //si ok -> invite_user() + RPL_INVITING

	}
} 