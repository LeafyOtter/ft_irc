#pragma once

#include <map>
#include <queue>
#include <set>
#include <vector>

#include <poll.h>
#include <stdint.h>

// used in User.hpp
#define U_MODE_REGISTERED		0x0001	// r : registered user
#define U_MODE_INVISIBLE		0x0002	// i : invisible
#define U_MODE_LOCAL_OPERATOR	0x0004	// o : local operator

// Used in Channel.hpp in users_t
#define CU_JOINED				0x0001	//   : user is in channel
#define CU_INVITED				0x0002	//   : user is invited to channel
#define CU_MODE_BANNED			0x0004	// b : user is banned from channel
#define CU_MODE_CHAN_OPERATOR	0x0008	// o : user is channel operator

// Used in Channel.hpp in mode
#define C_MODE_LIMIT			0x0001	// l : channel limit
#define C_MODE_INVITE_ONLY		0x0002	// i : invite only
#define C_MODE_KEY				0x0004	// k : channel requires key
#define C_MODE_SECRET			0x0008	// s : secret channel
#define C_MODE_TOPIC_LOCK		0x0010	// t : topic locked (can still be changed by op)

/**
 * @brief banned user
 *
 * Can still talk in channel, if U_MODE_CHAN_OPERATOR is set
 * Can still join channel, if invited by chanop
 */

#include "User.hpp"

namespace c_irc
{
	class Message;

	struct u_compare {
		bool operator()(const c_irc::User *lhs, const c_irc::User *rhs) const
		{
			return (lhs->get_nick() < rhs->get_nick());
		}
	};

	typedef typename std::queue<c_irc::Message *>						messages_t;
	typedef typename std::vector<pollfd>								pollfds_t;
	typedef typename std::map<c_irc::User*, uint16_t, c_irc::u_compare>	users_map_t;
	typedef typename std::set<c_irc::User*, c_irc::u_compare>			users_set_t;

	typedef typename users_map_t::iterator								users_map_it_t;
	typedef typename users_set_t::iterator								users_set_it_t;
} // namespace c_irc
