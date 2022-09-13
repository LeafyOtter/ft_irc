#include <cstring>	
#include <iostream>   
#include <vector>


namespace c_irc
{  
    inline void split(std::string str, char c, std::vector<std::string> &elements) // verif si c'est un soucis de mettre inline // sinon probleme de duplicate symbole
	{
		std::string str2;
		size_t pos = 0;
    
		while (pos != std::string::npos)
		{
			pos = str.find(c);
			str2 = str.substr(0, pos);
			str = str.substr(pos + 1);
			elements.push_back(str2);
		}
	}
}

// fonction channel existing ?
