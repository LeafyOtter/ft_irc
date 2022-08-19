#include "Channel.hpp"

namespace c_irc
{
	Channel::Channel(std::string new_name, c_irc::User *user)
		: name(new_name)
		, topic("")
		, key("")
		, mode(CU_MODE_CHAN_OPERATOR)
		, limit(0)
		, users()
	{
		users.insert(std::make_pair(user, mode));
	}

	Channel::~Channel()
	{
	}
} // namespace c_irc