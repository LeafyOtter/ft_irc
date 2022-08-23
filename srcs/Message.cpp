#include "Message.hpp"

namespace c_irc
{
	Message::Message(serv_users_t &u, chan_users_it_t first, chan_users_it_t last)
		: to_pop(false)
		, message(std::string())
		, target_type(TARGET_RANGE)
		, target(0)
		, first_target(first)
		, last_target(last)
		, sender(chan_users_it_t())
		, users(u)
	{}

	Message::Message(serv_users_t &u, int fd, std::string msg)
		: to_pop(false)
		, message(msg)
		, target_type(TARGET_UNREGISTERED)
		, target(fd)
		, first_target(chan_users_it_t())
		, last_target(chan_users_it_t())
		, sender(chan_users_it_t())
		, users(u)
	{}

	Message::Message(const Message &other)
		: to_pop(other.to_pop)
		, message(other.message)
		, target_type(other.target_type)
		, target(other.target)
		, first_target(other.first_target)
		, last_target(other.last_target)
		, sender(other.sender)
		, users(other.users)
	{}

	Message::~Message() {}

	Message &Message::operator=(const Message &other)
	{
		if (this != &other)
		{
			to_pop = other.to_pop;
			message = other.message;
			target_type = other.target_type;
			target = other.target;
			first_target = other.first_target;
			last_target = other.last_target;
			sender = other.sender;
		}
		return (*this);
	}

	std::string Message::get_message() const { return (message); }
	bool Message::get_status() { return (to_pop); }

	void Message::set_message(std::string new_message) { message = new_message; }
	void Message::set_sender(chan_users_it_t new_sender) { sender = new_sender; }
	void Message::set_status() { to_pop = true; }

	int Message::nb_users() const
	{

		int i = 0;

		if (target_type == TARGET_UNREGISTERED)
			return (1);
		for (chan_users_it_t it = first_target; it != last_target; ++it) {
			if (it != sender)
				++i;
		}
		return (i);
	}

	void	Message::prepare()
	{
		if (target_type == TARGET_UNREGISTERED) {
			users.at(target)->set_pollout();
			return ;
		}
		for (chan_users_it_t it = first_target; it != last_target; ++it) {
			if (it != sender)
				users.at(it->first)->set_pollout();
		}
	}
} // namespace c_irc
