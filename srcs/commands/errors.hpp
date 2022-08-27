#pragma once


#define ERR_NOSUCHCHANNEL(name) \
	":c-irc.net 403 " + name + " :invalide channel name\r\n"
#define ERR_NONICKNAMEGIVEN(nick) \
	":c-irc.net 431 " + nick + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, n) \
	":c-irc.net 432 " + nick + " " + n + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, n) \
	":c-irc.net 433 " + nick + " " + n + " :Nickname is already in use\r\n"
#define ERR_NEEDMOREPARAMS(nick, n) \
	":c-irc.net 461 " + nick + " " + n + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) \
	":c-irc.net 462 " + nick + " :Unauthorized command (already registered)\r\n"

