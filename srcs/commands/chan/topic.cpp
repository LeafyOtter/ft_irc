#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"
#include <string.h>

/*

   Numeric error and Replies:   * -> dans le fichier erreur // ** -> utilisé

           **ERR_NEEDMOREPARAMS              **ERR_NOTONCHANNEL
           **RPL_NOTOPIC                     **RPL_TOPIC
           **ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES (a verif)

        A FAIRE :
                - verif l'erreur nochanmode
                - :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the topic.
                - verifier le droit user avant de modifier le topic
                - reduir la fonctions
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
                std::string chan_name;
                std::string topic; 
                int pos; 

                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
                if (not (pos = channels.find(args[0]) != channels.end()))
                        return; // pas de message d'erreur ? 
                if (args.size() == 1)
                {
                        if(not channels[args[0]]->is_user_in_channel(nick))  
                        {
                                queue_message(ERR_NOTONCHANNEL(args[0]), fd);   
                                return;
                        }
                        topic = channels[args[0]]->get_topic(); 
                        if (topic.empty())
                                queue_message(RPL_NOTOPIC(args[0]), fd);    
                        else
                                queue_message(RPL_TOPIC(args[0], topic), fd);  
                }
                if (args.size() > 1) 
                {
                        //if(not channels[args[0]]->is_user_in_channel(nick))  --> NON verifier l'user a les droits
                        //{
                        //        queue_message(ERR_NOTONCHANNEL(args[0]), fd);   
                        //        return;
                        //}
                        if (pos = args[1].find(':') != 0)
                                return; 
                        if (args[1].size() == 1)
                        {
                                channels[args[0]]->set_topic(std::string()); 
                                return; 
                        }
                        else 
                        {
                                for (size_t i = 1; i < args.size(); i++)
                                        topic = topic + args[i]; 
                        }
                        channels[args[0]]->set_topic(topic);
                }
	}
} 