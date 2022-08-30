#pragma once

#include <string>

#define VERSION "0.0.1"

#define RPL_WELCOME(nick, username) \
	":c-irc.net 001 " + nick + " :Welcome to the Internet Relay Network " \
	+ username + "!" + username + "@c-irc.net\r\n"
#define RPL_YOURHOST(nick) \
	":c-irc.net 002 " + nick + " :Your host is c-irc.net, running version " \
	+ VERSION + "\r\n"
#define RPL_CREATED(nick, created) \
	":c-irc.net 003 " + nick + " :This server was created " + created + "\r\n"
#define RPL_MYINFO(nick) \
	":c-irc.net 004 " + nick + " :c-irc.net " + VERSION + " ior iklost\r\n"
#define RPL_AWAY(nick, message) \
	":c-irc.net 301 " + nick + " :" + message + "\r\n"
#define RPL_LIST(chan, count, topic) \
	":c-irc.net 322 " + chan + " " + count +  " :" + topic + "\r\n"
#define RPL_LISTEND(nick) \
	":c-irc.net 323 " + nick + " :End of /LIST \r\n"
#define RPL_NOTOPIC(chan) \
	":c-irc.net 331 " + chan + " :No topic is set\r\n"
#define RPL_TOPIC(chan, topic) \
	":c-irc.net 332 " + chan + " :" + topic + "\r\n"
#define RPL_INVITING(nick, chan) \
	":c-irc.net 341 " + nick + " " + chan + "\r\n"
#define RPL_NAMREPLY(nick_type, nick, chan_type, chan) \
	":c-irc.net 353 " + chan_type + chan +  " :" + nick_type + nick + "\r\n"	
#define RPL_ENDOFNAMES(nick, chan) \
	":c-irc.net 366 " + nick + " " + chan + " :End of /NAMES list\r\n"

//001    RPL_WELCOME
//       "Welcome to the Internet Relay Network
//        <nick>!<user>@<host>"
//002    RPL_YOURHOST
//       "Your host is <servername>, running version <ver>"
//003    RPL_CREATED
//       "This server was created <date>"
//004    RPL_MYINFO
//       "<servername> <version> <available user modes>
//        <available channel modes>"

//      - The server sends Replies 001 to 004 to a user upon
//        successful registration.




// MESSAGE INVENTE, OBLIGATOIRE POUR CMD PART (RFC 2851)
#define RPL_LEAVINGCHAN(nick, chan, reason) \
	":c-irc.net c-irc " + nick + " :is leaving the channel " + chan + " reason: " + reason + "\r\n"