#include "Server.hpp"
#include <iostream>


namespace c_irc
{
	void Server::cmd_part(int fd, arguments_t &args)
	{
		std::cout << "cmd part" << std::endl; 
		std::cout << fd << &args << std::endl; 
	}
} 