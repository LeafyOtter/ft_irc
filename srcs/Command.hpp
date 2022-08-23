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
	};
} // namespace c_irc