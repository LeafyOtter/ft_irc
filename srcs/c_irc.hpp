#pragma once

#define VERSION "version 0.0.1"
#define AUTHOR "John Doe"
#define GITHUB "https://github.com/LeafyOtter/ft_irc"

#define MOTD_FILE "./motd/motd_1.txt"

#include <map>
#include <queue>
#include <set>
#include <vector>

#include <sstream>
#include <string>

#include <poll.h>
#include <stdint.h>

// used in User.hpp
#define U_MODE_RESTRICTED		0x0001	// r : restricted user
#define U_MODE_INVISIBLE		0x0002	// i : invisible
#define U_MODE_OPERATOR			0x0004	// o : local operator
#define U_MODE_REGISTERED_PASS	0x0008	// client sent pass command (on registration)
#define U_MODE_REGISTERED_USER	0x0010	// client sent user command (on registration)
#define U_MODE_REGISTERED_NICK	0x0020	// client sent nick command (on registration)

// Used in Channel.hpp in mode
#define C_MODE_LIMIT			0x0001	// l : channel limit
#define C_MODE_INVITE_ONLY		0x0002	// i : invite only
#define C_MODE_KEY				0x0004	// k : channel requires key
#define C_MODE_SECRET			0x0008	// s : secret channel
#define C_MODE_TOPIC_LOCK		0x0010	// t : topic locked (can still be changed by op)
#define C_MODE_NO_EXTERNAL		0x0020	// n : no external messages
#define C_MODE_PRIVATE			0x0040	// p : private channel

// Colors
#define COLOR_BLACK				"\x1b[30m"
#define COLOR_RED				"\x1b[31m"
#define COLOR_GREEN				"\x1b[32m"
#define COLOR_YELLOW			"\x1b[33m"
#define COLOR_BLUE				"\x1b[34m"
#define COLOR_MAGENTA			"\x1b[35m"
#define COLOR_CYAN				"\x1b[36m"
#define COLOR_WHITE				"\x1b[37m"
#define COLOR_RESET				"\x1b[0m"

#include "User.hpp"

#define OPER_NAME				"c_irc_oper"
#define OPER_PASS				"hello42"

namespace c_irc
{
	class Message;
	class Channel;

	struct u_compare {
		bool operator()(const c_irc::User *lhs, const c_irc::User *rhs) const
		{
			return (lhs->get_nick() < rhs->get_nick());
		}
	};

	typedef std::queue<c_irc::Message *>				messages_t;
	typedef std::vector<pollfd>							pollfds_t;
	typedef std::vector<std::string>					list_users_t; // list banned and invited users
	typedef std::map<int, c_irc::User*>					serv_users_t;
	typedef std::map<int, bool>							chan_users_t;
	typedef std::map<std::string, c_irc::Channel *>		channels_t;

	typedef std::vector<std::string>					arguments_t;

	typedef serv_users_t::iterator						serv_users_it_t; //suit
	typedef chan_users_t::iterator						chan_users_it_t; //cuit
	typedef channels_t::iterator						channels_it_t;

	typedef list_users_t::iterator						list_it_t;
} // namespace c_irc
