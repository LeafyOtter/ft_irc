#pragma once

#include <string>

#define HOST "c_irc.net"

#define RPL_WELCOME(nick, username) "c_irc.net 001 " + nick + " :Welcome to the Internet Relay Network " + username + "!" + username + "@c_irc.net"
#define RPL_YOURHOST(host, version) HOST + " 002 " + host + " :Your host is " + host + ", running version " + version
#define RPL_CREATED(created) HOST + " 003 :This server was created " + created
#define RPL_MYINFO(host, version, usermodes, chanmodes) HOST + " 004 " + host + " " + version + " " + usermodes + " " + chanmodes + " :" + host


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