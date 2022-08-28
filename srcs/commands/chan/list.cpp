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
                - utiliser des iterateurs ou faire fonction dans channel pour compter le nombre d'users dans le channel --> apprendre ces putains d'itérateurs
                - trouver une autre solution pour convertir en string sans C++11
                - lister les chan demandé (apres avoir trouvé la fonction pour compter)
                - changer la fonction print channel_List
                
   
   A VERIF :
                - If the <target> parameter is specified, the request is forwarded to
                that server which will generate the reply.

                - Wildcards are allowed in the <target> parameter.
                - si le channel 2 n'existe, est ce qu'on affiche le channel 3 (oui)
                - quand utiliser RPL_LISTEND et ERR_NOSUCHSERVER

   Numeric Replies:

           ERR_TOOMANYMATCHES (a verif)         *ERR_NOSUCHSERVER
           **RPL_LIST                           *RPL_LISTEND


*/

namespace c_irc
{
        void Server::print_channel_List(c_irc::Channel &chan, int nb)
        {
                std::string chan_name = chan.get_name();  
                std::string topic = chan.get_topic();
                std::string count;  
        
                // trouver une autre solution
                std::stringstream num;
                num << nb;
                count = num.str();
                queue_message(RPL_LIST(chan_name, count, topic), fd);
        }    
        
        void Server::cmd_list(int fd, arguments_t &args)
	{
	        c_irc::User &user = *users[fd];
                int count = 0; 
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
		        return ;
		if (args.size() == 0) 
                {
                        std::cout << COLOR_CYAN << std::endl << "***** CHANNEL LIST ******" << COLOR_RESET << std::endl; // a supprimer
                        
                        // RETIRER CE PUTAIN DE TRUC IMMONDE --> En plus erreur compté le mb de channel et non le nombre d'user dans le channel
                        for ( channels_t::const_iterator it = channels.begin(); it != channels.end(); it++ ) 
                                count = count + 1; 
                        for ( channels_t::const_iterator it = channels.begin(); it != channels.end(); it++ ) 
                        {        //std::cout << it->first << std::endl; // utiliser RPL_LIST
                                c_irc::Channel &chan = *channels[it->first];
                                print_channel_List(chan, count); 
                        }
                }
                if (args.size() > 0)
                {
                        std::vector <std::string> element;
                                split(args[0], ',', element); 
                        for(size_t i = 0; i < element.size(); i++)
                        {
                                if (channels.find(element[i]) != channels.end()) 
                                {
                                        count = 0; // ici compter le nombre d'Users dans le channel
                                        //print_channel_List(channels[element[i]], count);   // n'inporte quoi
                                }  
                                      
			}
                }
	}
 }