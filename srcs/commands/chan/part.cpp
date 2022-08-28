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


    Numeric error and Replies:   * -> dans le fichier erreur // ** -> utilisé

           **ERR_NEEDMOREPARAMS              **ERR_NOSUCHCHANNEL
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
                std::string name = "PART";
                std::string nick = user.get_nick();
                std::string reason = "unknow"; 
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
                std::vector <std::string> element; 
                split(args[0], ',', element); 
                if (args.size() > 0)
                {
                        for (size_t i = 1; i < args.size(); i++)
                        reason = reason + args[i];  
                }
                for(size_t i = 0; i < element.size(); i++)
		{
                        if (channels.find(element[i]) != channels.end()) 
			{
				if(channels[element[i]]->is_user_in_channel(nick))
                                {
                                        channels[element[i]]->remove_user(fd);
                                        queue_message(RPL_LEAVINGCHAN(nick, element[i], reason), fd); // replies qui n'existe pas
                                }
                                else 
                                        queue_message(ERR_NOTONCHANNEL(element[i]), fd);
			}  
                        else 
                                queue_message(ERR_NOSUCHCHANNEL(element[i]), fd);
                }        
        }
} 