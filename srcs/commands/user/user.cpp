#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"

#include "utils.tpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_user(int fd, arguments_t &args)
	{
		(void)fd;
		(void)args;
		c_irc::User &user = *users[fd];

		users[fd]->set_user(c_irc::to_string(fd));


		if (not user.get_nick().empty() and \
			not user.get_user().empty())
		{
			LOG_USER(fd, "registered");
			if (not (user.get_mode() & U_MODE_WELCOMED))
			{
				LOG_USER(fd, "welcoming");
				std::string welcome;

				welcome  = RPL_WELCOME(user.get_nick(), user.get_user());
				welcome += RPL_YOURHOST(user.get_nick());
				welcome += RPL_CREATED(user.get_nick(), "[Placeholder]");
				welcome += RPL_MYINFO(user.get_nick());
				queue_message(welcome, fd);
			}
		}
	}
} // namespace c_irc
