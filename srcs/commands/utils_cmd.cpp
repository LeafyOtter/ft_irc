#include "Server.hpp"
#include "replies.hpp"
#include "utils.tpp"

#include <iostream>

namespace c_irc
{
	void Server::welcome(int fd)
	{
		std::string welcome;
		c_irc::User &user = *users[fd];
		
		if (not (user.is_mode(U_MODE_REGISTERED_NICK) and \
			user.is_mode(U_MODE_REGISTERED_USER)))
			return ;
		LOG_USER(fd, "welcoming");	
		welcome  = RPL_WELCOME(user.get_nick(), user.get_user());
		welcome += RPL_YOURHOST(user.get_nick());
		welcome += RPL_CREATED(user.get_nick(), creation_time);
		welcome += RPL_MYINFO(user.get_nick());
		queue_message(welcome, fd);
		user.unset_flag_mode(U_MODE_RESTRICTED);

		c_irc::arguments_t args;
		cmd_motd(fd, args);
	}
} // namespace c_irc