#include "Server.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage : " << av[0] << " <port>" << std::endl;
		return (1);
	}

	c_irc::Server server(av[1], "127.0.0.1", "password");

	server.initialize();
	server.run();
}