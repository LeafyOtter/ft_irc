#include "Server.hpp"
#include <iostream>


namespace c_irc
{
	void Server::cmd_list(int fd, arguments_t &args)
	{
		std::cout << COLOR_CYAN << std::endl << "***** CHANNEL LIST ******" << COLOR_RESET << std::endl; 
		std::cout << fd << &args << std::endl;


		for ( channels_t::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
    		std::cout << it->first << std::endl;}
	}
} 