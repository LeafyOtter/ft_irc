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

/*
							ERREURS POSSIBLES

  			**ERR_NEEDMOREPARAMS            ERR_BANNEDFROMCHAN
           	ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           	ERR_CHANNELISFULL               ERR_BADCHANMASK
           	**ERR_NOSUCHCHANNEL             ERR_TOOMANYCHANNELS
           	ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
           	RPL_TOPIC
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
		//c_irc::Channel &chan = *channels["test"];
		
		/*if (args[0] == "0")
		{
			foreach(channels)
				if user_name = user_name
					channel.remove_user(int fd);
			return; 
		}*/

		std::vector <std::string> element; 
		std::vector <std::string> key; 

		split(args[0], ',', element); 
		
		if (args.size() >= 2)
		//if (not args[1].empty())
			split(args[1], ',', key);

		// verifier les droits d'acces au chan et si pastrop de chan 

		/*for (element)
		{
			if (not is_name_valid(args[0]))
			{
				queue_message(ERR_NOSUCHCHANNEL(args[0]),fd);
				return; 
			}
			if (key not empty)
			{
				if (key(i))
					key = key[i]; 
				else 
					key = NULL; 		
			if (channels.find(chan_name) != channels.end()) {
        	std::cout << "channel exist" << std::endl;
		// ajouter l'user dedans 
			}
    		else 
			{
				//create_channel(chan_name, fd, std::string());
				create_channel(chan_name, fd, key);
				std::cout << "channel don't exist" << std::endl; 
			}	
			}
		}


*/

		if (not is_name_valid(args[0]))
		{
			queue_message(ERR_NOSUCHCHANNEL(args[0]),fd);
			return; 
		}
		chan_name = args[0]; 

    	if (channels.find(chan_name) != channels.end()) {
        	std::cout << "channel exist" << std::endl;
		// ajouter l'user dedans 
		}
    	else 
		{
			create_channel(chan_name, fd, std::string()); 
			std::cout << "channel don't exist" << std::endl; 
		}	







		// si chan existe rejoindre
		//sinon create channel
		
		

		
	}
} 