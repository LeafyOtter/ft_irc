#include "Server.hpp"

#include "../replies.hpp"
#include "../errors.hpp"

namespace c_irc
{
	void	Server::cmd_kill(int fd, arguments_t &args)
	{
		c_irc::User *user = users[fd];
		std::string nick = user->get_nick();

		if (users[fd]->is_mode(U_MODE_RESTRICTED))
		{
			queue_message(ERR_NOTREGISTERED(users[fd]->get_nick()), fd);
			return ;
		}

		if (args.size() < 2)
		{
			queue_message(ERR_NEEDMOREPARAMS(nick, "KILL"), fd);
			return ;
		}

		std::string victim = args[0]; // i love you copilot
		std::string reason = args[1];
		int victim_fd = is_user(victim);

		if (victim == "c-irc.net")
		{
			queue_message(ERR_CANTKILLSERVER(nick), fd);
			return ;
		}

		if (victim_fd == -1)
		{
			queue_message(ERR_NOSUCHNICK(nick, victim), fd);
			return ;
		}

		if (not user->is_mode(U_MODE_OPERATOR))
		{
			queue_message(ERR_NOPRIVILEGES(nick), fd);
			return ;
		}

		users[victim_fd]->set_delete();
		queue_message(RPL_KILL(nick, user->get_user(), victim, reason), victim_fd);
	}
} // namespace c_irc
