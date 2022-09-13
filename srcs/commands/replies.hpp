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

#define RPL_AWAY(nick, message) \
	":c-irc.net 301 " + nick + " :" + message + "\r\n"

#define RPL_LIST(nick, chan, count, topic) \
	":c-irc.net 322 " + nick + " " + chan + " " + count +  " :" + topic + "\r\n"
#define RPL_LISTEND(nick) \
	":c-irc.net 323 " + nick + " :End of /LIST \r\n"
#define RPL_CHANNELMODEIS(nick, chan, mode) \
	":c-irc.net 324 " + nick + " " + chan + " " + mode + "\r\n"

#define RPL_NOTOPIC(nick, chan) \
	":c-irc.net 331 " + nick + " " + chan + " :No topic is set\r\n"
#define RPL_TOPIC(nick, chan, topic) \
	":c-irc.net 332 " + nick + " " + chan + " :" + topic + "\r\n"

#define RPL_INVITING(nick, target, chan) \
	":c-irc.net 341 " + nick + " " + target + " " + chan + "\r\n"

#define RPL_VERSION(nick, server, version) \
	":c-irc.net 351 " + nick + " " + version + " :c-irc.net\r\n"

#define RPL_NAMREPLY(nick, chan, users) \
	":c-irc.net 353 " + nick + " " + chan + " :" + users + "\r\n"

#define RPL_ENDOFNAMES(nick, chan) \
	":c-irc.net 366 " + nick + " " + chan + " :End of /NAMES list\r\n"

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

/*
 *	Rebuilding needed commands.
 */

#define RPL_NICK(nick, user, new_nick) \
	":" + nick + "!" + user + "@c-irc.net NICK :" + new_nick + "\r\n"

#define RPL_PONG ":c-irc.net PONG c-irc.net\r\n"

#define RPL_JOIN(nick, user, chan) \
	":" + nick + "!" + user + "@c-irc.net JOIN " + chan + "\r\n"

#define RPL_PART(nick, user, chan, reason) \
	":" + nick + "!" + user + "@c-irc.net PART " + chan + " :" + reason + "\r\n"

#define RPL_PRIVMSG(nick, user, target, msg) \
	":" + nick + "!" + user + "@c-irc.net PRIVMSG " + target + " :" + msg + "\r\n"

#define RPL_TOPIC_CHANGE(nick, user, chan, topic) \
	":" + nick + "!" + user + "@c-irc.net TOPIC " + chan + " :" + topic + "\r\n"

#define RPL_ERROR \
	":c-irc.net ERROR :Disconnect\r\n"

#define RPL_NOTICE(nick, user, target, msg) \
	":" + nick + "!" + user + "@c-irc.net NOTICE " + target + " :" + msg + "\r\n"

#define RPL_CHAN_MODE(nick, user, channel, mode) \
	":" + nick + "!" + user + "@c-irc.net MODE " + channel + " " + mode + "\r\n"

#define RPL_KILL(nick, user, victim, reason) \
	":" + nick + "!" + user + "@c-irc.net KILL " + victim + " " + reason + "\r\n"

#define RPL_KICK(nick, user, channel, victim, reason) \
	":" + nick + "!" + user + "@c-irc.net KICK " + channel + " " + victim + " :" + reason + "\r\n"

#define RPL_QUIT(nick, user, reason) \
	":" + nick + "!" + user + "@c-irc.net QUIT :" + reason + "\r\n"

#define RPL_INVITED(nick, user, target, channel) \
	":" + nick + "!" + user + "@c-irc.net INVITE " + target + " :" + channel + "\r\n"

#define RPL_CAP \
	":c-irc.net CAP * LS:\r\n"

// MESSAGE INVENTE, OBLIGATOIRE POUR CMD PART (RFC 2851)
#define RPL_LEAVINGCHAN(nick, chan, reason) \
	":c-irc.net c-irc " + nick + " :is leaving the channel " + chan + " reason: " + reason + "\r\n"