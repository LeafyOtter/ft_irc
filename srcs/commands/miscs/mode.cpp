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
	// cmd_mode_user :
	//

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
			queue_message(RPL_UMODEIS(nick, umode_string(user.get_mode())), fd);
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

		// check if chan exists
		// loop on args
		// if flag is ikltob : get next arg
		/**
		 *	loop on args
		 *	{
		 *		loop on *args
		 *		{
		 *			if flag is ikltob : get next *(args + 1)
		 *			if args + 1 > args.size() ERR_NEEDMOREPARAMS
		 *			execute flag
		 *			erase (*args) + 1
		 *		}
		 *	}
		 */

	// cmd_mode_channel :
	// check if user is in channel
	// check if user is op
	// parse args
	// arguments possible :
	// -+i : invite list : invite-only channel		(chanop)	// no arg
	// -+k : key : set/remove key					(chanop)	// arg
	// -+l : limit : set/remove user limit			(chanop)	// arg
	// -+n : no messages from client outside		(chanop)	// no arg
	// -+t : topic can only be changed by chan op	(chanop)	// no arg
	// -+s : secret	: secret channel				(chanop)	// no arg
	// -+o : op										(chanop)	// arg

	//	Command: MODE
	//	Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

	enum mode_type_e
	{
		MODE_SET,
		MODE_UNSET,
		MODE_UNKNOWN
	};
	typedef enum mode_type_e mode_type_t;

	void Server::cmd_mode_chan(int fd, c_irc::User &user, arguments_t &args)
	{
		std::string		nick;
		std::string		name;
		channels_it_t	it;
		c_irc::Channel	*chan;
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

		// iklstobn
		size_t j = 0;
		while (not args.empty() and j < args.size())
		{
			LOG("args.size() : " << args.size());
			type = MODE_UNKNOWN;
			for (size_t i = 0, current = j; i < args[j].size(); i++) {
				switch (args[j][i])
				{
					case '+':
						type = MODE_SET;
						break;

					case '-':
						type = MODE_UNSET;
						break;

					case 'i':
						if (type == MODE_SET)
							chan->set_mode(C_MODE_INVITE_ONLY);
						else if (type == MODE_UNSET)
							chan->unset_mode(C_MODE_INVITE_ONLY);
						break ;

					case 'k':
						if (type == MODE_SET)
						{
							if (args.size() > current + 1)
							{
								chan->set_mode(C_MODE_KEY);
								chan->set_key(args[current + 1]);
								current++;
							}
						}
						else if (type == MODE_UNSET)
						{
							chan->unset_mode(C_MODE_KEY);
							chan->set_key("");
						}
						break ;
					case 'l':
						if (type == MODE_SET)
						{
							if (args.size() > current + 1)
							{
								chan->set_mode(C_MODE_LIMIT);
								chan->set_limit(c_irc::stoi(args[current + 1]));
								current++;
							}
						}
						else if (type == MODE_UNSET)
						{
							chan->unset_mode(C_MODE_LIMIT);
							chan->set_limit(0);
						}
						break ;
					case 'n':
						if (type == MODE_SET)
							chan->set_mode(C_MODE_NO_EXTERNAL);
						else if (type == MODE_UNSET)
							chan->unset_mode(C_MODE_NO_EXTERNAL);
						break ;
					case 't':
						if (type == MODE_SET)
							chan->set_mode(C_MODE_TOPIC_LOCK);
						else if (type == MODE_UNSET)
							chan->unset_mode(C_MODE_TOPIC_LOCK);
						break ;
					case 's':
						if (type == MODE_SET)
							chan->set_mode(C_MODE_SECRET);
						else if (type == MODE_UNSET)
							chan->unset_mode(C_MODE_SECRET);
						break ;
					case 'o':
						if (type == MODE_SET)
						{
							if (args.size() > current + 1)
							{
								int user_fd = chan->fd_from_nick(args[current + 1]);
								chan_users_it_t it = chan->get_user(user_fd);
								if (it != chan->end()) {
									chan->set_user_mode(user_fd);
									LOG("user_fd : " << user_fd << " set to op");
								}
								current++;
							}
						}
						else if (type == MODE_UNSET)
						{
							if (args.size() > current + 1)
							{
								int user_fd = c_irc::stoi(args[current + 1]);
								chan_users_it_t it = chan->get_user(user_fd);
								if (it != chan->end())
									chan->unset_user_mode(user_fd);
								current++;
							}
						}
						break ;
				default:
					break;
				}
			}
			j++;
		}
		queue_message(RPL_CHANNELMODEIS(nick, name, \
			cmode_string(chan->get_mode(), "")), fd);
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
