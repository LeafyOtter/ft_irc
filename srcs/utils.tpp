#pragma once

#include <sstream>
#include <string>

namespace c_irc
{
	std::string	get_time();
	int 		stoi(std::string str);
	std::string	to_upper(std::string str);

	template <typename T>
	std::string	to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return (os.str());
	}
} // namespace c_irc
