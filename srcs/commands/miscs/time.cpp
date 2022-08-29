#include "Server.hpp"
#include "utils.tpp"

#include "../replies.hpp"
#include "../errors.hpp"

namespace c_irc
{
	void	Server::cmd_time(int fd, arguments_t &args)
	{
		std::string nick = users[fd]->get_nick();

		if (args.size() != 0 and args[0] != "c-irc.net")
		{
			queue_message(ERR_NOSUCHSERVER(nick, args[0]), fd);
			return ;
		}
		queue_message(RPL_TIME(nick, c_irc::get_time()), fd);
	}
} // namespace c_irc
