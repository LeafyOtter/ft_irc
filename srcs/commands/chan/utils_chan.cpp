#include <cstring>	
#include <iostream>   
#include <vector>


namespace c_irc
{  
    void split(std::string str, char c, std::vector<std::string> &elements)
	{
		std::string str2;
		size_t pos = 0;
    
		while (pos != std::string::npos)
		{
			pos = str.find(c);
			str2 = str.substr(0, pos);
			str = str.substr(pos + 1);
			elements.push_back(str2);
			std::cout << str2 << std::endl;
		}
	}
}

// fonction channel existing ?
