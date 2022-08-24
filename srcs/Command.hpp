#pragma once

#include "c_irc.hpp"

namespace c_irc
{
	class Command
	{
		private:

		std::string		cmd;
		arguments_t		arguments;

		public:

		Command(std::string raw);
		~Command();

		std::string		get_cmd() const;
		std::string		get_arg(int index) const;
		arguments_t		&get_args();
		size_t			get_arg_count() const;

		void	parsing(std::string raw);
	};

	std::ostream& operator<<(std::ostream &os, const Command &cmd);
} // namespace c_irc