#pragma once



#define ERR_NOSUCHNICK(nick, type) \
	":c-irc.net 401 " + nick + " :No such " + type + "\r\n"
//#define ERR_NOSUCHSERVER(serv) \
//	":c-irc.net 402 " + serv + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(chan) \
	":c-irc.net 403 " + chan + " :No such channel\r\n"
#define ERR_TOOMANYCHANNEL(chan) \
	":c-irc.net 405 " + chan + " :You have joined too many channels\r\n"
#define ERR_NONICKNAMEGIVEN(nick) \
	":c-irc.net 431 " + nick + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, n) \
	":c-irc.net 432 " + nick + " " + n + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, n) \
	":c-irc.net 433 " + nick + " " + n + " :Nickname is already in use\r\n"
//#define ERR_UNAVAILRESSOURCE() 
//	error 437 "<nick/channel> :Nick/channel is temporarily unavailable" (pour nick et pour channel)
#define ERR_USERNOTINCHANNEL(nick, chan) \
	":c-irc.net 441 " + nick + " " + chan + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(chan) \
	":c-irc.net 442 " + chan + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(nick, chan) \
	":c-irc.net 443 " + nick + " " + chan + " :is already on channel\r\n"
#define ERR_NEEDMOREPARAMS(nick, n) \
	":c-irc.net 461 " + nick + " " + n + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) \
	":c-irc.net 462 " + nick + " :Unauthorized command (already registered)\r\n"
#define ERR_CHANNELISFULL(chan) \
	":c-irc.net 471 " + chan + " :Cannot join channel\r\n"// +1
#define ERR_INVITEONLYCHAN(chan) \
	":c-irc.net 473 " + chan + " :Cannot join channel\r\n" // +i
#define ERR_BANNEDFROMCHAN(chan) \
	":c-irc.net 474 " + chan + " :Cannot join channel\r\n" // +b
#define ERR_BADCHANNELKEY(chan) \
	":c-irc.net 475 " + chan + " :Cannot join channel\r\n" // +k
#define ERR_BADCHANMASK(chan) \
	":c-irc.net 476 " + chan + " :Cannot join channel\r\n" // +k
#define ERR_CHANOPRIVSNEEDED(chan) \
	":c-irc.net 482 " + chan + " :You're not channel operator\r\n"

