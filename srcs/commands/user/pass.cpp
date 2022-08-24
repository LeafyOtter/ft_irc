#include "Server.hpp"
#include "../errors.hpp"
#include "../replies.hpp"

namespace c_irc
{
	// Possible errors :
	// [ ] no password given
	// [ ] already registered

	void	Server::cmd_pass(int fd, arguments_t &args)
	{
		std::string name = "PASS";
		std::string password;
		c_irc::User &user = *users[fd];

		if (args.size() == 0)
		{
			queue_message(ERR_NEEDMOREPARAMS(name), fd);
			return ;
		}

		if (not user.get_mode() | U_MODE_RESTRICTED)
		{
			queue_message(ERR_ALREADYREGISTERED, fd);
			return ;
		}

		password = args[0];

		if (password != server.get_password())
		{
			queue_message(ERR_PASSWDMISMATCH, fd);
			return ;
		}
	}
}