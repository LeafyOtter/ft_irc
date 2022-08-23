#include "c_irc.hpp"
#include "Server.hpp"

#include <string>
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./server <ip> <port>" << std::endl;
		return (0);
	}

	c_irc::Server server;

		server.initialize("c_irc.net", av[1], av[2]);
		server.start();

	// try
	// {
	// 	server.initialize("c_irc.net", av[1], av[2]);
	// 	server.start();
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// 	return (1);
	// }
	return (0);
}