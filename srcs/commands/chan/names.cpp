#include "Server.hpp"
#include <iostream>


namespace c_irc
{
	void Server::cmd_names(int fd, arguments_t &args)
	{
		std::cout << "cmd names" << std::endl; 
		std::cout << fd << &args << std::endl; 
	}
} 