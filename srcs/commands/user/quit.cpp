#include "Server.hpp"

#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	void Server::cmd_quit(int fd, arguments_t &args)
	{
		std::string msg = args.size() >= 1 ? args[0] : "";
		std::string nick = users[fd]->get_nick();

		users[fd]->set_delete();
		// queue_message(RPL_ERROR, fd);

		for (channels_it_t it = channels.begin(); it != channels.end(); ++it)
		{
			if (it->second->get_number_of_users() == 1)
			{
				delete_channel(it->first);
				continue ;
			}
			if (it->second->is_user_in_channel(fd))
				queue_message(RPL_PART(nick, users[fd]->get_user(), it->first, msg), it->second);
		}
	}
} // namespace c_irc