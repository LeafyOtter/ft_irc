#include <sstream>

namespace c_irc
{
	std::string get_time()
	{
		time_t rawtime;
		struct tm *timeinfo;
		std::string ret;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		ret = asctime(timeinfo);
		return (ret);
	}

	int stoi(std::string str)
	{
		int i;
		std::stringstream ss(str);
		ss >> i;
		return (i);
	}

	std::string to_upper(std::string str)
	{
		for (size_t i = 0; i < str.size(); i++)
			str[i] = std::toupper(str[i]);
		return (str);
	}
} // namespace c_irc