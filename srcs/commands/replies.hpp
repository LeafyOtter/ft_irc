#pragma once

#include <string>

#define RPL_WELCOME(nick, username) \
	":c-irc.net 001 " + nick + " :Welcome to the Internet Relay Network " \
	+ nick + "!" + username + "@c-irc.net\r\n"
#define RPL_YOURHOST(nick) \
	":c-irc.net 002 " + nick + " :Your host is c-irc.net, running version " \
	+ VERSION + "\r\n"
#define RPL_CREATED(nick, created) \
	":c-irc.net 003 " + nick + " :This server was created " + created + "\r\n"
#define RPL_MYINFO(nick) \
	":c-irc.net 004 " + nick + " :c-irc.net " + VERSION + " ior iklost\r\n"

#define RPL_UMODEIS(nick, mode) \
	":c-irc.net 221 " + nick + " " + mode + "\r\n"

#define RPL_CHANNELMODEIS(nick, chan, mode) \
	":c-irc.net 324 " + nick + " " + chan + " " + mode + "\r\n"

#define RPL_VERSION(nick, server, version) \
	":c-irc.net 351 " + nick + " " + version + ".42 :c-irc.net\r\n"

#define RPL_INFO(nick, string) \
	":c-irc.net 371 " + nick + " :" + string + "\r\n"
#define RPL_MOTD(nick, line) \
	":c-irc.net 372 " + nick + " :" + line + "\r\n"

#define RPL_ENDOFINFO(nick) \
	":c-irc.net 374 " + nick + " :End of INFO list\r\n"

#define RPL_MOTDSTART(nick) \
	":c-irc.net 375 " + nick + " :- c-irc.net Message of the day -\r\n"
#define RPL_ENDOFMOTD(nick) \
	":c-irc.net 376 " + nick + " :End of MOTD command\r\n"

#define RPL_YOUREOPER(nick) \
	":c-irc.net 381 " + nick + " :You are now an IRC operator\r\n"

#define RPL_TIME(nick, time) \
	":c-irc.net 391 " + nick + " :" + time + "\r\n"

#define RPL_NICK(nick, user, new_nick) \
	":" + nick + "!" + user + "@c-irc.net NICK :" + new_nick + "\r\n"

#define RPL_PONG ":c-irc.net PONG c-irc.net\r\n"

#define RPL_PRIVMSG(nick, user, target, msg) \
	":" + nick + "!" + user + "@c-irc.net PRIVMSG " + target + " :" + msg + "\r\n"

#define RPL_ERROR \
	":c-irc.net ERROR :Disconnect\r\n"

#define RPL_NOTICE(nick, user, target, msg) \
	":" + nick + "!" + user + "@c-irc.net NOTICE " + target + " :" + msg + "\r\n"

#define RPL_CHAN_MODE(nick, user, channel, mode) \
	":" + nick + "!" + user + "@c-irc.net MODE " + channel + " " + mode + "\r\n"

#define RPL_KILL(nick, user, victim, reason) \
	":" + nick + "!" + user + "@c-irc.net KILL " + victim + " :" + reason + "\r\n"

// :otter!otter_0x2e3@ircserv.42 MODE otter +i
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