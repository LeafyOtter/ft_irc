#include "Server.hpp"
#include "../errors.hpp"

namespace c_irc
{

	// TODO: move thoses functions to a separate file

	bool is_special(char c)
	{
		if (c >= 0x5b and c <= 0x60)
			return (true);
		if (c >= 0x7b and c <= 0x7d)
			return (true);
		return (false);
	}

	bool is_digit(char c)
	{
		if (c >= '0' and c <= '9')
			return (true);
		return (false);
	}

	bool is_letter(char c)
	{
		if ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z'))
			return (true);
		return (false);
	}

	bool is_nickname_valid(const std::string &nickname)
	{
		if (nickname.size() > 9)
			return (false);
		if (not is_letter(nickname[0]) and not is_special(nickname[0]))
			return (false);
		for (size_t i = 1; i < nickname.size(); i++)
			if (not is_letter(nickname[i]) and not is_digit(nickname[i]) and
				not is_special(nickname[i]) and not nickname[i] == '-')
				return (false);
		return (true);
	}

	// Possible errors :
	// [x] no nickname given
	// [x] nickname already used
	// [x] erroneous nickname
	// [x] restricted connection

	void Server::cmd_nick(int fd, arguments_t &args)
	{
		std::string nick;
		c_irc::User &user = *users[fd];

		if (user.get_mode() | U_MODE_RESTRICTED)
		{
			queue_message(ERR_RESTRICTED, fd);
			return ;
		}

		if (args.empty()) {
			queue_message(ERR_NONICKNAMEGIVEN, fd);
			return ;
		}

		nick = args[0];

		for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it)
		{
			if ((*it).first != fd and (*it).second->get_nick() == nick)
			{
				queue_message(ERR_NICKNAMEINUSE, fd);
				return ;
			}
		}

		if (not is_nickname_valid(nick))
		{
			queue_message(ERR_ERRONEUSNICKNAME(nick), fd);
			return ;
		}

		user.set_nick(nick);
	}
} // namespace c_irc