#pragma once

#define ERR_NONICKNAMEGIVEN(nick) \
	":c_irc 431 " + nick + " 	:No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, n) \
	":c_irc 432 " + nick + " " + n + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, n) \
	":c_irc 433 " + nick + " " + n + " :Nickname is already in use\r\n"

#define ERR_NEEDMOREPARAMS(nick, n) \
	":c_irc 461 " + nick + " " + n + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) \
	":c_irc 462 " + nick + " :Unauthorized command (already registered)\r\n"
#define ERR_RESTRICTED(nick) \
	":c_irc 484 " + nick + " :You are restricted from this server\r\n"
