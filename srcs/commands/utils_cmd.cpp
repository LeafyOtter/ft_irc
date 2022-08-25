#include "Server.hpp"
#include "replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::welcome(int fd)
	{
		std::string welcome;
		c_irc::User &user = *users[fd];
		
		if (not(user.get_mode() & U_MODE_REGISTERED_NICK) and \
			not (user.get_mode() & U_MODE_REGISTERED_USER))
			return ;
		LOG_USER(fd, "welcoming");	
		welcome  = RPL_WELCOME(user.get_nick(), user.get_user());
		welcome += RPL_YOURHOST(user.get_nick());
		welcome += RPL_CREATED(user.get_nick(), "[Placeholder]");
		welcome += RPL_MYINFO(user.get_nick());
		queue_message(welcome, fd);
		user.set_mode(user.get_mode() & ~U_MODE_RESTRICTED);
	}
} // namespace c_irc