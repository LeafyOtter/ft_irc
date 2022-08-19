#pragma once

#include <sstream>
#include <string>

namespace c_irc
{
	int 		stoi(std::string str)
	{
		int i;
		std::stringstream ss(str);
		ss >> i;
		return (i);
	}

	template <typename T>
	std::string	to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return (os.str());
	}
} // namespace c_irc
