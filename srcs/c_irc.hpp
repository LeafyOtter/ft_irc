#pragma once

#include <map>

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

class User;

typedef std::map<User*, uint16_t>	users_t;