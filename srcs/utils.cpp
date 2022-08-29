#include <sstream>

namespace c_irc
{
	int stoi(std::string str)
	{
		int i;
		std::stringstream ss(str);
		ss >> i;
		return (i);
	}

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
} // namespace c_irc