#pragma once

#define ERR_NOSUCHSERVER(nick, serv) \
	":c-irc.net 401 " + nick + " " + serv + " :No such server\r\n"

#define ERR_NOSUCHCHANNEL(nick, chan) \
	":c-irc.net 403 " + nick + " " + chan + " :No such channel\r\n"

#define ERR_NOORIGIN(nick) \
	":c-irc.net 421 " + nick + " :No origin specified\r\n"

#define ERR_NONICKNAMEGIVEN(nick) \
	":c-irc.net 431 " + nick + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, n) \
	":c-irc.net 432 " + nick + " " + n + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, n) \
	":c-irc.net 433 " + nick + " " + n + " :Nickname is already in use\r\n"

#define ERR_USERNOTINCHANNEL(nick, target, chan) \
	":c-irc.net 441 " + nick + " " + target + " " + chan + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(nick, chan) \
	":c-irc.net 442 " + nick + " " + chan + " :You're not on that channel\r\n"

#define ERR_NEEDMOREPARAMS(nick, n) \
	":c-irc.net 461 " + nick + " " + n + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) \
	":c-irc.net 462 " + nick + " :Unauthorized command (already registered)\r\n"

#define ERR_UNKNOWNMODE(nick, mode, chan) \
	":c-irc.net 463 " + nick + " " + mode + \
	" :Is unknown mode char to me for " + chan + "\r\n"

#define ERR_PASSWDMISMATCH(nick) \
	":c-irc.net 464 " + nick + " :Password incorrect\r\n"

#define ERR_CHANOPRIVISNEEDED(nick, chan) \
	":c-irc.net 482 " + nick + " " + chan + " :You're not channel operator\r\n"

#define ERR_UMODEUNKNOWNFLAG(nick) \
	":c-irc.net 501 " + nick + " :Unknown MODE flag\r\n"
#define ERR_USERDONTMATCH(nick) \
	":c-irc.net 502 " + nick + " :Cannot change mode for other users\r\n"