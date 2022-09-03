#include "Message.hpp"

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

namespace c_irc
{
	Message::Message(serv_users_t &u, std::string msg, int target_fd)
		: to_pop(false)
		, utarget(target_fd)
		, ctarget(NULL)
		, target_type(TARGET_USER)
		, sender(0)
		, message(msg)
		, users(u)
	{}

	Message::Message(serv_users_t &u, std::string msg, c_irc::Channel *c, int sender_fd)
		: to_pop(false)
		, utarget(0)
		, ctarget(c)
		, target_type(TARGET_CHANNEL)
		, sender(sender_fd)
		, message(msg)
		, users(u)
	{}

	Message::Message(const Message &other)
		: to_pop(other.to_pop)
		, utarget(other.utarget)
		, ctarget(other.ctarget)
		, target_type(other.target_type)
		, sender(other.sender)
		, message(other.message)
		, users(other.users)
	{}

	Message::~Message() {}

	Message &Message::operator=(const Message &other)
	{
		if (this != &other)
		{
			to_pop = other.to_pop;
			utarget = other.utarget;
			ctarget = other.ctarget;
			target_type = other.target_type;
			sender = other.sender;
			message = other.message;
			users = other.users;
		}
		return (*this);
	}

	std::string Message::get_message() const { return (message); }
	bool Message::get_status() { return (to_pop); }

	void Message::set_message(std::string new_message) { message = new_message; }
	void Message::set_sender(int new_sender) { sender = new_sender; }
	void Message::set_status() { to_pop = true; }

	void Message::append_message(std::string new_message) { message += new_message; }

	int Message::nb_targets() const
	{
		if (target_type == TARGET_USER)
			return (1);
		else if (target_type == TARGET_CHANNEL)
			return (ctarget->get_number_of_users() - (sender != 0 ? 1 : 0));
		return (0);
	}

	void	Message::prepare()
	{
		if (target_type == TARGET_USER)
		{
			users.at(utarget)->set_pollout();
			return ;
		}
		for (chan_users_it_t it = ctarget->begin(); it != ctarget->end(); ++it)
		{
			if (it->first != sender)
				users[it->first]->set_pollout();
		}
	}
} // namespace c_irc
