#include "Command.hpp"

namespace c_irc
{
	Command::Command(std::string raw)
	{
		parsing(raw);
	}

	Command::~Command()
	{
		arguments.clear();
	}

	void Command::parsing(std::string raw)
	{
		size_t pos;
		size_t pos_end;
		std::string tmp;
		std::string delimiter;


		delimiter = ": ";
		pos = raw.find_first_of(delimiter);
		cmd = raw.substr(0, pos);
		if (pos == std::string::npos)
			return ;
		while (0xCAFE)
		{
			raw = raw.substr(pos + 1);
			if (raw[0] == ':')
			{
				delimiter = ":";
				pos_end = raw.find_first_of(delimiter, 1);
				if (pos_end != std::string::npos)
					tmp = raw.substr(1, pos_end - 1);
				else
					tmp = raw.substr(1);
			}
			else
			{
				delimiter = " :";
				pos = raw.find_first_not_of(delimiter);
				if (pos == std::string::npos)
					break ;
				pos_end = raw.find_first_of(delimiter, pos);
				tmp = raw.substr(pos, pos_end - pos);
			}
			arguments.push_back(tmp);
			pos = pos_end;
			if (pos == std::string::npos)
				break ;
		}
	}

	std::string Command::get_cmd() const { return (cmd); }
	std::string Command::get_arg(int index) const { return (arguments[index]); }
	arguments_t &Command::get_args() { return (arguments); }
	size_t 		Command::get_arg_count() const { return (arguments.size()); }

	std::ostream &operator<<(std::ostream &os, const Command &cmd)
	{
		os << "Cmd : [" << cmd.get_cmd() << "]" << std::endl;
		for (size_t i = 0; i < cmd.get_arg_count(); i++)
			os << "\tArg : [" << cmd.get_arg(i) << "]" << std::endl;
		return (os);
	}
} // namespace c_irc