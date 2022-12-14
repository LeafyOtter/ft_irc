#include "Channel.hpp"

namespace c_irc
{
	Channel::Channel(serv_users_t &su, std::string new_name, int new_fd)
		: name(new_name)
		, topic("")
		, key("")
		, mode(0)
		, limit(0)
		, ban_list()
		, invite_list()
		, chan_users()
		, serv_users(su)
	{
		chan_users.insert(std::make_pair(new_fd, true));
	}

	Channel::~Channel()
	{
		chan_users.clear();
		ban_list.clear();
		invite_list.clear();
	}

	std::string Channel::get_name() const { return (name); }
	std::string Channel::get_topic() const { return (topic); }
	std::string Channel::get_key() const { return (key); }
	uint16_t Channel::get_mode() const { return (mode); }
	uint16_t Channel::get_limit() const { return (limit); }
	size_t	Channel::get_number_of_users() const { return chan_users.size();}

	void Channel::set_topic(std::string new_topic) { topic = new_topic; }
	void Channel::set_key(std::string new_key) { key = new_key; }
	void Channel::set_mode(uint16_t new_mode) { mode |= new_mode; }
	void Channel::unset_mode(uint16_t new_mode) { mode &= ~new_mode; }
	void Channel::set_limit(uint16_t new_limit) { limit = new_limit; }

	void Channel::set_user_mode(int fd, uint16_t new_mode)
	{
		chan_users[fd] |= new_mode;
	}

	void Channel::unset_user_mode(int fd, uint16_t new_mode)
	{
		chan_users[fd] &= ~new_mode;
	}

	bool Channel::is_mode(uint16_t mode) const { return (this->mode & mode); }

	void Channel::set_user_mode(int fd) { chan_users[fd] = true; }
	void Channel::unset_user_mode(int fd) { chan_users[fd] = false; }
	bool Channel::is_user_op(int fd) { return (chan_users[fd]);}

	void Channel::add_user(int id)
	{
		chan_users.insert(std::make_pair(id, 0));
	}

	void Channel::remove_user(int id)
	{
		chan_users.erase(id);
	}

	void Channel::remove_user(std::string new_user)
	{
		chan_users_it_t next;

		for (chan_users_it_t it = begin(); it != end(); it = next)
		{
			next = it;
			next++;

			if (serv_users[it->first]->get_nick() == new_user)
				chan_users.erase(serv_users[it->first]->get_fd());
		}
	}

	void Channel::invite_user(std::string new_user)
	{
		invite_list.push_back(new_user);
	}

	void Channel::uninvite_user(std::string new_user)
	{
		for (list_it_t it = invite_list.begin(); it != invite_list.end(); ++it)
		{
			if (*it == new_user) {
				invite_list.erase(it);
				return ;
			}
		}
	}

	bool Channel::is_user_invited(std::string new_user)
	{
		for (list_it_t it = invite_list.begin(); it != invite_list.end(); ++it)
			if (*it == new_user)
				return (true);
		return (false);
	}

	bool Channel::is_user_in_channel(int fd)
	{
		return (chan_users.find(fd) != chan_users.end());
	}

	bool Channel::is_user_in_channel(std::string new_user)
	{
		for (chan_users_it_t it = begin(); it != end(); ++it)
			if (serv_users.at(it->first)->get_nick() == new_user)
				return (true);
		return (false);
	}

	bool  Channel::is_mode(uint16_t fl) { return (mode & fl); }

	int Channel::fd_from_nick(std::string new_user)
	{
		for (chan_users_it_t it = begin(); it != end(); ++it)
			if (serv_users.at(it->first)->get_nick() == new_user)
				return (it->first);
		return (-1);
	}

	bool Channel::is_empty() const { return (chan_users.empty()); }
	bool Channel::is_full() const { return (chan_users.size() == limit); }

	c_irc::chan_users_it_t Channel::begin() { return (chan_users.begin()); }
	c_irc::chan_users_it_t Channel::end() { return (chan_users.end()); }

	c_irc::chan_users_it_t Channel::get_user(int fd)
	{
		for (chan_users_it_t it = begin(); it != end(); ++it)
			if (it->first == fd)
				return (it);
		return (end());
	}
} // namespace c_irc