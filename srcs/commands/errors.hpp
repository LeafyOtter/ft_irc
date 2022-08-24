#pragma once

#define ERR_NONICKNAMEGIVEN ":c_irc 431 :No nickname given"
#define ERR_ERRONEUSNICKNAME(x) ":c_irc 432 " + x + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE ":c_irc 433 :Nickname is already in use"

#define ERR_NEEDMOREPARAMS(x) ":c_irc 461 " + x + " :Not enough parameters"
#define ERR_ALREADYREGISTERED ":c_irc 462 :Unauthorized command (already registered)"

#define ERR_RESTRICTED ":c_irc 484 :You are restricted from this server"