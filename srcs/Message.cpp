#include "Message.hpp"

namespace c_irc
{
	Message::Message(users_it first, users_it last)
		: message("")
		, first(first)
		, last(last)
		, to_pop(false) {}

	Message::Message(std::string msg, users_it first, users_it last)
		: message(msg)
		, first(first)
		, last(last)
		, to_pop(false) {}

	Message::~Message() {}

	std::string Message::get_message() const { return (message); }
	void Message::set_status() { to_pop = true; }
	bool Message::get_status() { return (to_pop); }
	void Message::set_message(std::string new_message) { message = new_message; }
	void Message::set_sender(users_it new_sender) { sender = new_sender; }

	int Message::nb_users() const {
		int i = 0;

		for (users_it it = first; it != last; ++it) {
			if (it != sender)
				++i;
		}
		return (i);
	}

	void	Message::prepare() {
		for (users_it it = first; it != last; ++it) {
			if (it != sender)
				(*it)->set_pollout();
		}
	}

} // namespace c_irc
