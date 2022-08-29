#include "c_irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "utils.tpp"

#include "../errors.hpp"
#include "../replies.hpp"

#include <iostream>

#define LOG(x) std::cout << x << std::endl;

namespace c_irc
{
	enum mode_type_e
	{
		MODE_SET,
		MODE_UNSET,
		MODE_UNKNOWN
	};
	typedef enum mode_type_e mode_type_t;

	static
	std::string umode_string(uint16_t mode)
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

	static
	void u_set_unset(User *user, uint16_t mode, mode_type_t type)
	{
		if (type == MODE_SET)
		{
			if (U_MODE_OPERATOR & mode)
				return ;
			user->set_flag_mode(mode);
		}
		else if (type == MODE_UNSET)
		{
			if (U_MODE_RESTRICTED & mode)
				return ;
			user->unset_flag_mode(mode);
		}
	}

	void Server::cmd_mode_user(int fd, c_irc::User &user, arguments_t &args)
	{
		std::string nick = user.get_nick();
		mode_type_t type = MODE_UNKNOWN;

		if (args[0] != nick)
		{
			queue_message(ERR_USERDONTMATCH(nick), fd);
			return ;
		}
		args.erase(args.begin());
		if (args.size() == 0)
		{
			queue_message(RPL_UMODEIS(nick, umode_string(user.get_mode())), fd);
			return ;
		}
		for (size_t i = 0; i < args[0].size(); i++)
		{
			switch (args[0][i])
			{
				case '+':
					type = MODE_SET;
					break;

				case '-':
					type = MODE_UNSET;
					break;

				case 'i':
					u_set_unset(&user, U_MODE_INVISIBLE, type);
					break;

				case 'o':
					u_set_unset(&user, U_MODE_OPERATOR, type);
					break;

				case 'r':
					u_set_unset(&user, U_MODE_RESTRICTED, type);
					break;

				default:
					queue_message(ERR_UMODEUNKNOWNFLAG(nick), fd);
					return ;
			}
		}
		queue_message(RPL_UMODEIS(nick, umode_string(user.get_mode())), fd);
	}

	static
	std::string	cmode_string(uint16_t flag, std::string pass)
	{
		std::string ret = "+";

		if (flag & C_MODE_INVITE_ONLY)
			ret += "i";
		if (flag & C_MODE_KEY)
			ret += "k";
		if (flag & C_MODE_LIMIT)
			ret += "l";
		if (flag & C_MODE_NO_EXTERNAL)
			ret += "n";
		if (flag & C_MODE_SECRET)
			ret += "s";
		if (flag & C_MODE_TOPIC_LOCK)
			ret += "t";
		if (pass.size() > 0)
			ret += " " + pass;
		return (ret);
	}

	static void c_set_unset(Channel &chan, uint16_t flag, mode_type_t type)
	{
		if (type == MODE_SET)
			chan.set_mode(flag);
		else if (type == MODE_UNSET)
			chan.unset_mode(flag);
	}

	void Server::cmd_mode_chan(int fd, c_irc::User &user, arguments_t &args)
	{
		size_t 			current = 0;
		c_irc::Channel	*chan;
		channels_it_t	it;
		std::string		nick;
		std::string		name;
		mode_type_t		type = MODE_UNKNOWN;

		LOG_USER(fd, "cmd_mode_chan");

		nick = user.get_nick();
		name = args[0];
		it = channels.find(name);
		args.erase(args.begin());

		if (it == channels.end())
		{
			queue_message(ERR_NOSUCHCHANNEL(nick, name), fd);
			return ;
		}

		chan = it->second;
		if (not chan->is_user_in_channel(nick))
		{
			queue_message(ERR_NOTONCHANNEL(nick, name), fd);
			return ;
		}

		if (args.size() == 0)
		{
			LOG_USER(fd, "cmd_mode_chan : no args");
			queue_message(RPL_CHANNELMODEIS(nick, name, \
				cmode_string(chan->get_mode(), chan->get_key())), fd);
			return ;
		}

		if (not chan->is_user_op(fd))
		{
			queue_message(ERR_CHANOPRIVISNEEDED(nick, name), fd);
			return ;
		}

		for (size_t j = 0; j < args.size(); j = current + 1, current = j)
		{
			type = MODE_UNKNOWN;
			for (size_t i = 0; i < args[j].size(); i++) {
				switch (args[j][i])
				{
					case '+':
						type = MODE_SET;
						break;

					case '-':
						type = MODE_UNSET;
						break;

					case 'i':
						c_set_unset(*chan, C_MODE_INVITE_ONLY, type);
						break ;

					case 'k':
						c_set_unset(*chan, C_MODE_KEY, type);
						if (args.size() > ++current and type == MODE_SET)
							chan->set_key(args[current]);
						else
							chan->set_key("");
						break ;

					case 'l':
						c_set_unset(*chan, C_MODE_LIMIT, type);
						if (args.size() > ++current and type == MODE_SET)
							chan->set_limit(c_irc::stoi(args[current]));
						else
							chan->set_limit(0);
						break ;

					case 'n':
						c_set_unset(*chan, C_MODE_NO_EXTERNAL, type);
						break ;

					case 't':
						c_set_unset(*chan, C_MODE_TOPIC_LOCK, type);
						break ;

					case 's':
						c_set_unset(*chan, C_MODE_SECRET, type);
						break ;

					case 'o':
						int new_fd;

						if (type == MODE_UNKNOWN)
							break ;
						if (args.size() < current + 1)
							return ;
						new_fd = chan->fd_from_nick(args[current]);
						if (new_fd == -1)
						{
							queue_message(ERR_USERNOTINCHANNEL(nick, args[current], name), fd);
							continue ;
						}

						if (type == MODE_SET)
							chan->set_user_mode(new_fd);
						else
							chan->unset_user_mode(new_fd);
						break ;

				default:
					queue_message(ERR_UNKNOWNMODE(nick, args[j][i], name), fd);
					return ;
				}
			}
		}
		queue_message(RPL_CHANNELMODEIS(nick, name, \
			cmode_string(chan->get_mode(), "")), fd);
		// probably need to redo this
		queue_message(RPL_CHAN_MODE(nick, user.get_user(), name, \
			cmode_string(chan->get_mode(), "")), chan->begin(), chan->end(), \
			chan->get_user(fd));
	}


	void Server::cmd_mode(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
		std::string	chan_identifiers = "&#+!";

		LOG_USER(fd, "cmd_mode");
		if (args.size() < 1)
		{
			queue_message(ERR_NEEDMOREPARAMS(user.get_nick(), "MODE"), fd);
		}
		if (chan_identifiers.find_first_of(args[0][0]) != std::string::npos)
			cmd_mode_chan(fd, user, args);
		else
		{
			LOG_USER(fd, user);
			cmd_mode_user(fd, user, args);
			LOG_USER(fd, user);
		}
		// print all modes from user
	}
} // namespace c_irc
