#include "Server.hpp"
#include <iostream>


namespace c_irc
{
	void Server::cmd_kick(int fd, arguments_t &args)
	{
		std::cout << "cmd kick" << std::endl;
		std::cout << fd << &args << std::endl; 
	}
} 