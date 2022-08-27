#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

namespace c_irc
{
	#define UNKNOWN	0x01
	#define SET_I	0x02
	#define SET_O	0x04
	#define SET_R	0x08
	#define UNSET_I	0x10
	#define UNSET_O	0x20
	#define UNSET_R	0x40

	uint16_t extract_argument(std::string &str)
	{
		bool stop = false;
		bool unset = true;
		uint16_t ret = 0;

		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] == '-')
			{
				if (stop)
				{
					str = str.substr(i);
					return (ret);
				}
				unset = true;
				stop = true;
				continue ;
			}
			if (str[i] == '+')
			{
				if (stop)
				{
					str = str.substr(i);
					return (ret);
				}
				unset = false;
				stop = true;
				continue ;
			}
			switch (str[i])
			{
			case 'i':
				ret |= unset ? UNSET_I : SET_I;
				break;
			case 'o':
				ret |= unset ? UNSET_O : SET_O;
				break;
			case 'r':
				ret |= unset ? UNSET_R : SET_R;
				break;
			default:
				ret |= UNKNOWN;
				break;
			}
		}
		str = "";
		return (ret);
	}

	// cmd_mode_channel :
	// check if user is in channel
	// check if user is op
	// parse args
	// arguments possible :
	// -+i : invite list
	// -+k : key
	// -+l : limit
	// -+t : topic
	// -+s : secret
	// -+o : op
	// -+b : ban

	void Server::cmd_mode_chan(int fd, c_irc::User &user, arguments_t &args)
	{
		(void)fd;
		(void)user;
		(void)args;
		LOG_USER(fd, "cmd_mode_chan");
	}

	// cmd_mode_user :
	//

	static
	std::string mode_string(uint16_t mode)
	{
		std::string ret = "+";

		if (mode & U_MODE_INVISIBLE)
			ret += "i";
		if (mode & U_MODE_OPERATOR)
			ret += "o";
		if (mode & U_MODE_RESTRICTED)
			ret += "r";
		return (ret);
	}

	void Server::cmd_mode_user(int fd, c_irc::User &user, arguments_t &args)
	{
		std::string nick = user.get_nick();
		uint16_t flag;
		std::string msg;
		bool unknown = false;

		if (args[0] != nick)
		{
			queue_message(ERR_USERDONTMATCH(nick), fd);
			return ;
		}
		args.erase(args.begin());
		if (args.size() == 0)
		{
			queue_message(RPL_UMODEIS(nick, mode_string(user.get_mode())), fd);
			return ;
		}
		while (not args[0].empty())
		{
			flag = extract_argument(args[0]);
			if (flag & UNKNOWN)
			{
				if (not unknown)
					msg += ERR_UMODEUNKNOWNFLAG(nick);
				unknown = true;
				LOG_USER(fd, "Unknown mode flag");
			}
			if (flag & SET_I)
			{
				user.set_flag_mode(U_MODE_INVISIBLE);
				LOG_USER(fd, "set mode invisible");
			}
			if (flag & SET_O)
				; // ignore
			if (flag & SET_R)
			{
				user.set_flag_mode(U_MODE_RESTRICTED);
				LOG_USER(fd, "restricted mode");
			}
			if (flag & UNSET_I)
			{
				user.unset_flag_mode(U_MODE_INVISIBLE);
				LOG_USER(fd, "unset invisible");
			}
			if (flag & UNSET_O)
			{
				user.unset_flag_mode(U_MODE_OPERATOR);
				LOG_USER(fd, "unset operator");
			}
			if (flag & UNSET_R)
				; // ignore
		}
		queue_message(RPL_UMODEIS(nick, mode_string(user.get_mode())), fd);
	} 

	void Server::cmd_mode(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::string	chan_identifiers = "&#+!";

		LOG_USER(fd, "cmd_mode");
		LOG_USER(fd, user);
		if (args.size() < 1)
		{
			queue_message(ERR_NEEDMOREPARAMS(user.get_nick(), "MODE"), fd);
		}
		if (chan_identifiers.find_first_of(args[0][0]) != std::string::npos)
			cmd_mode_chan(fd, user, args);
		else
			cmd_mode_user(fd, user, args);
		// print all modes from user
		LOG_USER(fd, user);
	}
} // namespace c_irc
