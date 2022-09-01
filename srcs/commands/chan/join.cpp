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
		size_t pos = 0;

		nick = user.get_nick();

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

		if (args[0] == "0")
		{
			// loop on all chans and remove user
			return ;
		}

		while (pos != std::string::npos)
		{
			key = "";
			if (args.size() >= 2)
			{
				pos = args[1].find(',');
				key = args[1].substr(0, pos);
				args[1] = pos != std::string::npos ? args[1].substr(pos + 1) : "";
			}

			pos = args[0].find(',');
			chan_name = args[0].substr(0, pos);
			args[0] = pos != std::string::npos ? args[0].substr(pos + 1) : "";

			if (channels.find(chan_name) == channels.end())
			{
				create_channel(chan_name, fd, key);
				chan = channels[chan_name];
				msg += RPL_JOIN(nick, user.get_user(), chan_name);
				msg += print_users_list(chan, fd, nick);
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

			if (chan->is_user_in_channel(nick))
				continue ;

			chan->add_user(fd);
			queue_message(RPL_JOIN(nick, user.get_user(), chan_name), chan, fd);
			msg += RPL_JOIN(nick, user.get_user(), chan_name);
			msg += print_users_list(chan, fd, nick);
		}
		queue_message(msg, fd);
	}
}