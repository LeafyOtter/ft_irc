#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"
#include "../replies.hpp"
#include "../../Channel.hpp"
#include <sstream> // a retirer apres 
#include<string>
/*

  Command: LIST
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   A FAIRE :
                - trouver une autre solution pour convertir en string sans C++11 
                   
   A VERIF :
                - quand utiliser ERR_NOSUCHSERVER (multiserver ?)
                
   Numeric Replies:

           ERR_TOOMANYMATCHES (a verif)         *ERR_NOSUCHSERVER (pour les tagets) ??
           **RPL_LIST                           **RPL_LISTEND


*/

namespace c_irc
{
    //     void Server::print_channel_list(Channel *chan, int fd, std::string nick)
    //     {
    //             std::string name = chan->get_name();  
    //             std::string topic = chan->get_topic();
    //             size_t count = chan->get_number_of_users();  
    //             std::string count_users; 
        
                
    //             // trouver une autre solution pour passer en string 
    //             std::stringstream num;
    //             num << count;
    //             count_users = num.str();

    //             if (chan->is_mode(C_CHAN_PROTECTED)) && !(chan->is_user_in_channel(nick)))         // RFC 1459
    //                     queue_message(RPL_LIST(name, count_users, "channel \"Prv\""), fd);
    //             else          
    //                     queue_message(RPL_LIST(name, count_users, topic), fd);
    //     }       
        
    //     void Server::cmd_list(int fd, arguments_t &args)
	// {
	//         c_irc::User &user = *users[fd];
    //             std::map<std::string, c_irc::Channel *>::iterator it; 
    //             std::string nick;

	// 	nick = user.get_nick();
    //             if (not user.is_mode(U_MODE_REGISTERED_PASS))
	// 	        return ;
	// 	if (args.size() == 0) 
    //             {
    //                     for (it = channels.begin(); it != channels.end(); it++ ) 
    //                             if (! (it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick))  // RFC 1459
    //                                     print_channel_list(it->second, fd, nick); 
    //             }
    //             if (args.size() > 0)
    //             {
    //                     std::vector <std::string> element;
    //                             split(args[0], ',', element); 
    //                     for(size_t i = 0; i < element.size(); i++)
    //                     {
    //                             if ((it = channels.find(element[i])) != channels.end()) 
    //                                     if (! (it->second->is_mode(C_MODE_SECRET)) || it->second->is_user_in_channel(nick)) 
    //                                             print_channel_list(it->second, fd, nick);          
	// 		}
    //             }
    //             queue_message(RPL_LISTEND(nick), fd); 
	// }
	void Server::cmd_list(int fd, arguments_t &args)
	{
		(void)fd;
		(void)args;
	}
}