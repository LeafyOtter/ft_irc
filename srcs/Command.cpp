#include "Command.hpp"

namespace c_irc
{
	Command::Command(std::string raw)
	{
		// Split the raw command into the command and the arguments
		// use either space or colon as separator
		size_t pos = raw.find_first_of(" :");
		std::string tmp;

		if (pos == std::string::npos)
		{
			cmd = raw;
			return ;
		}
		cmd = raw.substr(0, pos - 1);
		raw = raw.substr(pos - 1);
		while (0xCAFE)
		{
			if (raw[0] == ':')
			{
				pos = raw.find(':');
				if (pos == std::string::npos)
					tmp = raw.substr(1);
				else
					tmp = raw.substr(1, pos - 1);
				arguments.push_back(tmp);
				if (pos == std::string::npos)
					break ;
				continue ;
			}
			pos = raw.find_first_of(" :");

			arguments.push_back(raw.substr(0, pos));
		}
	}
} // namespace c_irc
