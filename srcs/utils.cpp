#pragma once

#include <sstream>

namespace c_irc
{
	int 	stoi(std::string str)
	{
		int i;
		std::stringstream ss(str);
		ss >> i;
		return (i);
	}
} // namespace c_irc