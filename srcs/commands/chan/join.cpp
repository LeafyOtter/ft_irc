#include "Server.hpp"
#include "../errors.hpp"
#include "../../Channel.hpp"
#include "../replies.hpp"
#include <iterator>
#include <cstring>
#include <vector>
#include <string>

#include "utils.tpp"
#include <iostream>

/*	A FAIRE
	- gerer JOIN 0 --> sortir de tous les chan (voir PART)
	- gerer si channel plein --> ERR_CHANNELISFULL 
	- gerer si le user a bien un mode operator a la creation du channel
	- Si un JOIN réussit, l'utilisateur reçoit un message JOIN en guise de confirmation
	- gerer l'envoi de la cle sur un chan avec cle qui existe deja -> cle incorrect ou non fournie, differente de invalid key qui est une erreur a lq definition de la cle ERR_BADCHANNELKEY
	- certain chan en invite only,  ERR_INVITEONLYCHAN 
	- gerer si l'utilisateur est banni ERR_BANNEDFROMCHAN
	- gerer si l'utilisateur est sur trop de chan  ERR_TOOMANYCHANNELS
	- envoyer le message RPL_TOPIC quand on rejoint un channel 
	- erreur TOOMANY target
		407    ERR_TOOMANYTARGETS
              "<target> :<error code> recipients. <abort message>"

        Returned to a client which is attempting to send a
        PRIVMSG/NOTICE using the user@host destination format
        and for a user@host which has several occurrences.

        Returned to a client which trying to send a
        PRIVMSG/NOTICE to too many recipients.

        Returned to a client which is attempting to JOIN a safe
    	channel using the shortname when there are more than one
        such channel.
	- erreur UNAVAILRESSOURCE --> - Returned by a server to a user trying to join a channel
        currently blocked by the channel delay mechanism.

        Returned by a server to a user trying to change nickname
    	when the desired nickname is blocked by the nick delay
        mechanism.

	PLUS TARD 
	
	- 	Une fois qu'un utilisateur a rejoint un canal, il reçoit des informations sur toutes les commandes reçues par son serveur concernant le canal. (canal qu'il a créé ??)
		Cela inclut JOIN, MODE, KICK, PART, QUIT et bien sûr PRIVMSG/NOTICE

	- l'utilisateur doit recevoir le sujet du chan (TOPIC) + liste des utilisateur sur le canal (NAME ?)

	????
	- :WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone ; JOIN message from WiZ
                                   on channel #Twilight_zone
	- Les serveurs DOIVENT être capables d'analyser les arguments sous la forme d'une liste de cibles, 
	mais NE DEVRAIENT PAS utiliser de listes lors de l'envoi de messages JOIN aux clients.
	
	- quand mettre cette erreur :  	ERR_NOSUCHCHANNEL --> channel qui n'existe pas alors que join creait les channels qui n'existent pas 
*/

/*
							ERREURS POSSIBLES  *entree dans le fichier erreur **utilisee

  			**ERR_NEEDMOREPARAMS            *ERR_BANNEDFROMCHAN
           	*ERR_INVITEONLYCHAN             *ERR_BADCHANNELKEY
           	*ERR_CHANNELISFULL              **ERR_BADCHANMASK
           	*ERR_NOSUCHCHANNEL             	*ERR_TOOMANYCHANNELS
           	ERR_TOOMANYTARGETS  			ERR_UNAVAILRESOURCE (pas trouve)
           	
			*RPL_TOPIC
*/

namespace c_irc
{

	bool is_name_valid(std::string new_name)
	{
		std::string start = "#&+!";

		if (new_name.size() > 50)
			return (false);
		if (start.find(new_name[0]) == std::string::npos)
			return (false);
		if (new_name.find_first_of(" \t,:;") != std::string::npos)
			return (false);
		if (new_name.find("^G") != std::string::npos)
			return (false);
		return (true);
	}

	// void split(std::string str, char c, std::vector<std::string> &elements)
	// {
	// 	std::string str2;
	// 	size_t db = 0;
    
	// 	for(size_t i = 0; i <= str.size(); i++)
    // 	{    
	// 		if(str[i] == c || i == str.size())
	// 		{
	// 			str2 = str.substr(db, i - db); 
	// 			db = i + 1; 
 	// 			elements.push_back(str2);  
	// 		}
	// 	}
	// }

	void split(std::string str, char c, std::vector<std::string> &elements)
	{
		std::string str2;
		size_t pos = 0;
    
		while (pos != std::string::npos)
		{
			pos = str.find(c);
			str2 = str.substr(0, pos);
			str = str.substr(pos + 1);
			elements.push_back(str2);
			std::cout << str2 << std::endl;
		}

	}

	void Server::cmd_join(int fd, arguments_t &args)
	{
		std::string name = "JOIN";
		std::string chan_name; 
		std::string nick; 
		c_irc::User &user = *users[fd];
		nick = user.get_nick();
		if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}
		if (args[0] == "0")
		{
			// remove user ou voir PART 
			return; 
		}

		// ici verification des droits d'acces user, si channel full
		std::vector <std::string> element; 
		std::vector <std::string> key_tab; 
		std::string key = ""; 
		
		split(args[0], ',', element); 
		if (args.size() >= 2)
			split(args[1], ',', key_tab);
		for(size_t i = 0; i < element.size(); i++)
		{
			std::cout << element[i] << std::endl; 
			if (not is_name_valid(element[i]))
			{
				queue_message(ERR_BADCHANMASK(element[i]),fd);
				return; 
			}
			chan_name = element[i]; 
			if (not key.empty())
			{
				if (i >= key.size())
					key = key_tab[i]; 
			}	
			if (channels.find(chan_name) != channels.end()) 
			{
        		std::cout << "channel exist" << std::endl; // a supprimer
				channels[chan_name]->add_user(fd);
			}
    		else 
			{
				create_channel(chan_name, fd, key);
				std::cout << "channel don't exist" << std::endl; // a supprimer
			}	
		}
	}
}


// MON BORDEL 


		// a verifier : si user donne une key sur un channel existant 
		//				si user donne un nom de chanel invalid en deuxieme position, doit on creer ceux d'apres

	/*		if (not is_name_valid(args[0]))
		{
			queue_message(ERR_NOSUCHCHANNEL(args[0]),fd);
			return; 
		}
		chan_name = args[0]; 

    	if (int pos = channels.find(chan_name) != channels.end()) {
        	std::cout << "channel exist" << std::endl;
			channels[chan_name]->add_user(fd);
		}
    	else 
		{
			create_channel(chan_name, fd, std::string()); 
			std::cout << "channel don't exist" << std::endl; 
		}	
	//c_irc::Channel &chan = *channels["test"];
	*/