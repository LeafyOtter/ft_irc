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

	std::string Channel::get_name() const { return (name); }
	std::string Channel::get_topic() const { return (topic); }
	std::string Channel::get_key() const { return (key); }
	uint16_t Channel::get_mode() const { return (mode); }
	uint16_t Channel::get_limit() const { return (limit); }

	void Channel::set_topic(std::string new_topic) { topic = new_topic; }
	void Channel::set_key(std::string new_key) { key = new_key; }
	void Channel::set_mode(uint16_t new_mode) { mode = new_mode; }
	void Channel::set_limit(uint16_t new_limit) { limit = new_limit; }

	void Channel::add_user(c_irc::User* new_user)
	{
		users.insert(std::make_pair(new_user, 0));
	}

	void Channel::remove_user(c_irc::User* new_user)
	{
		users.erase(new_user);
	}

	bool Channel::is_name_valid(std::string new_name)
	{
		(void)new_name;
		// TODO: implement
		return (true);
	}

	bool Channel::is_user_banned(std::string new_user)
	{
		(void)new_user;
		// TODO: implement
		return (false);
	}

	bool Channel::is_user_in_channel(std::string new_user)
	{
		for (users_it_t it = users.begin(); it != users.end(); ++it)
			if (it->first->get_nick() == new_user)
				return (true);
		return (false);
	}

	c_irc::users_it_t Channel::begin() { return (users.begin()); }
	c_irc::users_it_t Channel::end() { return (users.end()); }
} // namespace c_irc