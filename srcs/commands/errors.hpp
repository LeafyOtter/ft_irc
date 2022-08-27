#pragma once

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


#define ERR_UMODEUNKNOWNFLAG(nick) \
	":c-irc.net 501 " + nick + " :Unknown MODE flag\r\n"
#define ERR_USERDONTMATCH(nick) \
	":c-irc.net 502 " + nick + " :Cannot change mode for other users\r\n"