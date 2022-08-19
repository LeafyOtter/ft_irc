#include "Message.hpp"

namespace c_irc
{
	Message::Message(users_it_t first, users_it_t last)
		: to_pop(false)
		, message(std::string())
		, target_type(TARGET_RANGE)
		, target(NULL)
		, first_target(first)
		, last_target(last)
		, sender(users_it_t())
	{}

	Message::Message(std::string msg, users_it_t first, users_it_t last)
		: to_pop(false)
		, message(msg)
		, target_type(TARGET_RANGE)
		, target(NULL)
		, first_target(first)
		, last_target(last)
		, sender(users_it_t())
	{}

	Message::Message(std::string msg, pollfd *new_pfd)
		: to_pop(false)
		, message(msg)
		, target_type(TARGET_UNREGISTERED)
		, target(new_pfd)
		, first_target(users_it_t())
		, last_target(users_it_t())
		, sender(users_it_t())
	{}

	Message::Message(const Message &other)
		: to_pop(other.to_pop)
		, message(other.message)
		, target_type(other.target_type)
		, target(other.target)
		, first_target(other.first_target)
		, last_target(other.last_target)
		, sender(other.sender)
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
	void Message::set_status() { to_pop = true; }
	bool Message::get_status() { return (to_pop); }
	void Message::set_message(std::string new_message) { message = new_message; }
	void Message::set_sender(users_it_t new_sender) { sender = new_sender; }

	int Message::nb_users() const {

		int i = 0;

		if (target_type == TARGET_UNREGISTERED)
			return (1);
		for (users_it_t it = first_target; it != last_target; ++it) {
			if (it != sender)
				++i;
		}
		return (i);
	}

	void	Message::prepare() {
		if (target_type == TARGET_UNREGISTERED) {
			target->events |= POLLOUT;
			return ;
		}
		for (users_it_t it = first_target; it != last_target; ++it) {
			if (it != sender)
				(*it).first->set_pollout();
		}
	}
} // namespace c_irc
