#pragma once

#include <string>

#define VERSION "0.0.1"

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

#define RPL_TIME(nick, time) \
	":c-irc.net 391 " + nick + " :" + time + "\r\n"

#define RPL_PONG ":c-irc.net PONG c-irc.net\r\n"

#define RPL_CHAN_MODE(nick, user, channel, mode) \
	":" + nick + "!" + user + "@c-irc.net MODE " + channel + " " + mode + "\r\n"

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