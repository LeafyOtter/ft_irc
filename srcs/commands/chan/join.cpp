#include "Server.hpp"
#include "../errors.hpp"
#include "../../Channel.hpp"
#include "../replies.hpp"
#include <iterator>
#include <cstring>
#include <vector>
#include <string>

#include "utils_chan.cpp"
#include <iostream>

/*	A FAIRE
				
	deja sur le channel ??
	
	A FAIRE :
			- JOIN 0
			- verification utilisation de trop de chan user 
			- ajouter une fonction inavlid key quand mauvaise syntaxe de cle : 
				key  =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
                  		; any 7-bit US_ASCII character,
                  		; except NUL, CR, LF, FF, h/v TABs, and " "
			- verifier si channel full
			- utilisateur banni -> a ne pas mettre
			- envoyer un message quand channel join (RPL_TOPIC)


	A VERIF :
			- invalid cle n'est pas dans les erreurs
			- Si un JOIN réussit, l'utilisateur reçoit un message JOIN en guise de confirmation differente pour l'operator et le user ? 
			- Une fois qu'un utilisateur a rejoint un canal, il reçoit des informations sur toutes les commandes reçues par son serveur concernant le canal. (canal qu'il a créé ??)
			- l'utilisateur doit recevoir le sujet du chan (TOPIC) + liste des utilisateur sur le canal (NAME ?)
				This includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
			- erreur TOOMANY target
				407    ERR_TOOMANYTARGETS "<target> :<error code> recipients. <abort message>"
			- Returned to a client which is attempting to send a
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
			- :WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone ; JOIN message from WiZ on channel #Twilight_zone
			- Les serveurs DOIVENT être capables d'analyser les arguments sous la forme d'une liste de cibles, 
			mais NE DEVRAIENT PAS utiliser de listes lors de l'envoi de messages JOIN aux clients.
			- quand mettre cette erreur :  	ERR_NOSUCHCHANNEL --> channel qui n'existe pas alors que join creait les channels qui n'existent pas 
*/

/*
							ERREURS POSSIBLES  *entree dans le fichier erreur **utilisee

  			**ERR_NEEDMOREPARAMS            **ERR_BANNEDFROMCHAN
           	*ERR_INVITEONLYCHAN             **ERR_BADCHANNELKEY
           	**ERR_CHANNELISFULL             **ERR_BADCHANMASK
           	*ERR_NOSUCHCHANNEL             	**ERR_TOOMANYCHANNELS
           	ERR_TOOMANYTARGETS (a verif) 	ERR_UNAVAILRESOURCE (a verif)
           	
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

	// bool Server::channel_autorization(std::string element, int fd, std::string key)
	// {
	// 		int pos; 
	// 		if (not is_name_valid(element))
	// 		{
	// 			queue_message(ERR_BADCHANMASK(element),fd); // nom invalide != channel inexistant
	// 			return 0; 
	// 		}
	// 		//if (not is_name_key_valid)
	// 		//{
	// 		//	return 0; // a verif car erreur non mentionnee : ERR_INVALIDKEY (dans channel ?)
	// 		//}
			
	// 		//if (verifier si l'user a trop de chan)
	// 		//{
	// 		//	queue_message(ERR_TOOMANYCHANNELS(element), fd);  
	// 		//	 return 0;
	// 		//}
	// 		if (channels.find(element) != channels.end()) 
	// 		{
	// 			if (channels[element]->get_key() != key)
	// 			{
	// 				queue_message(ERR_BADCHANNELKEY(element),fd); 
	// 				return 0;
	// 			}
	// 			if(channels[element]->is_mode(INVITE_ONLY))
	// 			{
	// 				queue_message(ERR_INVITEONLYCHAN (element),fd); 
	// 				return 0;
	// 			}
	// 			//if si channel plein / limite
	// 			//{ 
	// 			//	queue_message(ERR_CHANNELISFULL(element),fd); 
	// 			//	return 0 
	// 			//} 
			
	// 			//envoyer le message RPL_TOPIC quand on rejoint un channel (user simple et operateur different ?)
	// 			channels[element]->add_user(fd);
	// 			return (0); 
	// 		}
    // 		return 1; 
	// }

	// void Server::parse_cmd_join(arguments_t &args, int fd, std::string chan_name)
	// {	
	// 	std::vector <std::string> element; 
	// 	std::vector <std::string> key_tab; 
	// 	std::string key = ""; 
		
	// 	split(args[0], ',', element); 
	// 	if (args.size() >= 2)
	// 		split(args[1], ',', key_tab);

	// 	for(size_t i = 0; i < element.size(); i++)
	// 	{
	// 		std::cout << element[i] << std::endl; 
	// 		chan_name = element[i]; 
	// 		if (not key.empty())
	// 		{
	// 			if (i >= key.size())
	// 				key = key_tab[i]; 
	// 		}	
	// 		if (channel_autorization(chan_name, fd, key))   // ok channel inexistant a creer 
	// 		{
	// 			create_channel(chan_name, fd, key);
	// 			// ajouter que l'user est en mode operator ?
	// 			//envoi un message pour confir;er (voir message operateur et user simple)				
	// 		}
	// 	}
	// }


	// void Server::cmd_join(int fd, arguments_t &args)
	// {
	// 	std::string name = "JOIN";
	// 	std::string chan_name; 
	// 	std::string nick; 
	// 	c_irc::User &user = *users[fd];

	// 	if (not user.is_mode(U_MODE_REGISTERED_PASS))
	// 		return ;
	// 	nick = user.get_nick();
	// 	if (args.empty())
	// 	{
	// 		queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
	// 		return ;
	// 	}
	// 	if (args[0] == "0")
	// 	{
	// 		// iteration sur channels et verif si l'user y est pour le supprimer usser.move()
	// 		return; 
	// 	}
	// 	//- gerer si l'utilisateur est sur trop de chan  ERR_TOOMANYCHANNELS
	// 		//return;
	// 	parse_cmd_join(args, fd, chan_name);  
	// }	

	// ERR_NOTREGISTERED : User is not registered
	// ERR_NEEDMOREPARAMS : The given command requires more parameters.
	// ERR_INVITEONLYCHAN : The given channel is invite only. (and user is not on invite list)
	// ERR_BADCHANNELKEY : The given channel key is invalid.
	// ERR_CHANNELISFULL : The given channel is full. (limit)
	// ERR_NOSUCHCHANNEL : The given channel does not exist. ??? When do we get this error ?
	// ERR_TOOMANYCHANNELS : The user has joined too many channels. (limist must be configured in like a conf file)
	// ^ no need to implement this error imo

#define LOG(x) std::cout << x << std::endl;

	void Server::cmd_join(int fd, arguments_t &args)
	{
		std::string name = "JOIN";
		std::string chan_name;
		std::string key;
		std::string nick;
		std::string msg;
		c_irc::Channel *chan;
		c_irc::User &user = *users[fd];

		nick = user.get_nick();

		LOG("cmd_join");

		if (user.is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(nick), fd);
			return ;
		}

		LOG("not restricted");

		if (args.empty())
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, name), fd);
			return ;
		}

		LOG("args not empty");

		if (args[0] == "0")
		{
			// loop on all chans and remove user
			return ;
		}

		LOG("args not 0");

		while (0xCAFE)
		{
			chan_name = args[0].substr(0, args[0].find_first_of(','));
			args[0] = args[0].substr(args[0].find_first_of(',') + 1);
			if (args.size() >= 2)
			{
				key = args[1].substr(0, args[1].find_first_of(','));
				args[1] = args[1].substr(args[1].find_first_of(',') + 1);
			}

			if (chan_name.empty())
				break ;

			LOG("chan_name : " << chan_name << " key : " << key);
			if (channels.find(chan_name) == channels.end())
			{
				create_channel(chan_name, fd, key);
				queue_message(RPL_JOIN(nick, user.get_user(), chan_name), fd);
				continue ;
			}

			chan = channels[chan_name];

			if (chan->is_mode(C_MODE_INVITE_ONLY) && !chan->is_user_invited(nick))
			{
				queue_message(ERR_INVITEONLYCHAN(nick, chan_name), fd);
				return ;
			}

			if (chan->is_mode(C_MODE_KEY) and key != chan->get_key())
			{
				queue_message(ERR_BADCHANNELKEY(nick, chan_name), fd);
				return ;
			}

			if (chan->is_full())
			{
				queue_message(ERR_CHANNELISFULL(nick, chan_name), fd);
				return ;
			}

			chan->add_user(fd);
			queue_message(RPL_JOIN(nick, user.get_user(), chan_name), \
				chan->begin(), chan->end(), chan->get_user(fd));
			msg += RPL_JOIN(nick, user.get_user(), chan_name);
		}
		queue_message(msg, fd);
	}
}