#include "User.hpp"
#include <vector>

namespace c_irc
{
	typedef std::vector<pollfd>		pollfds_t;

	User::User(int new_fd, pollfds_t &new_pfds)
		: nick("")
		, user("")
		, host("")
		, realname("")
		, mode(0)
		, fd(new_fd)
		, pfds(new_pfds)
		, pfds_index(0)
	{
		get_pollfd();	// initialize pfds_index
	}

	User::~User() {}

	std::string User::get_nick() const { return (nick); }
	std::string User::get_user() const { return (user); }
	std::string User::get_host() const { return (host); }
	std::string User::get_realname() const { return (realname); }
	uint16_t User::get_mode() const { return (mode); }
	int User::get_fd() const { return (fd); }

	void User::set_nick(std::string new_nick) { nick = new_nick; }
	void User::set_user(std::string new_user) { user = new_user; }
	void User::set_host(std::string new_host) { host = new_host; }
	void User::set_realname(std::string new_realname) { realname = new_realname; }
	void User::set_mode(uint16_t new_mode) { mode = new_mode; }

	void User::set_flag_mode(uint16_t flag) { mode |= flag; }
	void User::unset_flag_mode(uint16_t flag) { mode &= ~flag; }

	void User::set_pollout()
	{
		pollfd *pfd = get_pollfd();
		pfd->events |= POLLOUT;
	}

	void User::unset_pollout()
	{
		pollfd *pfd = get_pollfd();
		pfd->events &= ~POLLOUT;
	}

	pollfd *User::get_pollfd()
	{
		if (pfds_index and pfds_index < pfds.size())
			if (pfds[pfds_index].fd == fd)
				return (&pfds[pfds_index]);
		for (size_t i = 0; i < pfds.size(); i++) {
			if (pfds[i].fd == fd) {
				pfds_index = i;
				return (&pfds[i]);
			}
		}
		return (NULL);
	}
} // namespace c_irc
