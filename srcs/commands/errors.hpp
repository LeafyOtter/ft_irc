#pragma once

#define ERR_NOSUCHNICK(nick, target) \
	":c-irc.net 401 " + nick + " " + target + " :No such nick/channel\r\n"
#define ERR_NOSUCHSERVER(nick, serv) \
	":c-irc.net 402 " + nick + " " + serv + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(nick, chan) \
	":c-irc.net 403 " + nick + " " + chan + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, chan) \
	":c-irc.net 404 " + nick + " " + chan + " :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNEL(nick, chan) \
	":c-irc.net 405 " + nick + " "chan + " :You have joined too many channels\r\n"

#define ERR_NORECIPIENT(nick, cmd) \
	":c-irc.net 411 " + nick + " " + cmd + " :No recipient given\r\n"
#define ERR_NOTEXTTOSEND(nick) \
	":c-irc.net 412 " + nick + " :No text to send\r\n"

#define ERR_NOORIGIN(nick) \
	":c-irc.net 421 " + nick + " :No origin specified\r\n"
#define ERR_NOMOTD(nick) \
	":c-irc.net 422 " + nick + " :MOTD File is missing\r\n"

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
#define ERR_USERONCHANNEL(nick, target, chan) \
	":c-irc.net 443 " + nick + " " + target + " " + chan + " :is already on channel\r\n"

#define ERR_NOTREGISTERED(nick) \
	":c-irc.net 451 " + nick + " :You have not registered\r\n"

#define ERR_NEEDMOREPARAMS(nick, n) \
	":c-irc.net 461 " + nick + " " + n + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) \
	":c-irc.net 462 " + nick + " :Unauthorized command (already registered)\r\n"
#define ERR_UNKNOWNMODE(nick, mode, chan) \
	":c-irc.net 463 " + nick + " " + mode + \
	" :Is unknown mode char to me for " + chan + "\r\n"
#define ERR_PASSWDMISMATCH(nick) \
	":c-irc.net 464 " + nick + " :Password incorrect\r\n"

#define ERR_CHANNELISFULL(nick, chan) \
	":c-irc.net 471 " + nick + " " + chan + " :Cannot join channel\r\n"// +1
#define ERR_INVITEONLYCHAN(nick, chan) \
	":c-irc.net 473 " + nick + " " + chan + " :Cannot join channel\r\n" // +i
#define ERR_BANNEDFROMCHAN(nick, chan) \
	":c-irc.net 474 " + nick + " " + chan + " :Cannot join channel\r\n" // +b
#define ERR_BADCHANNELKEY(nick, chan) \
	":c-irc.net 475 " + nick + " " + chan + " :Cannot join channel\r\n" // +k
#define ERR_BADCHANMASK(nick, chan) \
	":c-irc.net 476 " + nick + " " + chan + " :Cannot join channel\r\n" // +k

#define ERR_NOPRIVILEGES(nick) \
	":c-irc.net 481 " + nick + " :Permission Denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, chan) \
	":c-irc.net 482 " + nick + " " + chan + " :You're not channel operator\r\n"
#define ERR_CANTKILLSERVER(nick) \
	":c-irc.net 483 " + nick + " :You can't kill a server!\r\n"

#define ERR_UMODEUNKNOWNFLAG(nick) \
	":c-irc.net 501 " + nick + " :Unknown MODE flag\r\n"
#define ERR_USERDONTMATCH(nick) \
	":c-irc.net 502 " + nick + " :Cannot change mode for other users\r\n"

