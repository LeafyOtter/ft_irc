#include "User.hpp"

namespace c_irc
{
	User::User(pollfd *new_pfd)
		: nick("")
		, user("")
		, host("")
		, realname("")
		, mode(0)
		, pfd(new_pfd) {}

	User::~User() {}

	std::string User::get_nick() const { return (nick); }
	std::string User::get_user() const { return (user); }
	std::string User::get_host() const { return (host); }
	std::string User::get_realname() const { return (realname); }
	uint16_t User::get_mode() const { return (mode); }
	pollfd *User::get_pfd() const { return (pfd); }

	void User::set_nick(std::string new_nick) { nick = new_nick; }
	void User::set_user(std::string new_user) { user = new_user; }
	void User::set_host(std::string new_host) { host = new_host; }
	void User::set_realname(std::string new_realname) { realname = new_realname; }
	void User::set_mode(uint16_t new_mode) { mode = new_mode; }

	void User::set_flag_mode(uint16_t flag) { mode |= flag; }
	void User::unset_flag_mode(uint16_t flag) { mode &= ~flag; }

	void User::set_pollout() { pfd->events |= POLLOUT; }
	void User::unset_pollout() { pfd->events &= ~POLLOUT; }

	bool compare(const c_irc::User &lhs, const  c_irc::User &rhs)
	{ return (lhs.get_nick() < rhs.get_nick()); }
} // namespace c_irc
