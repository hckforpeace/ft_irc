# include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << RED << "Wrong number of arguments. Run as ./ircserver <port> <password>" << RESET << std::endl;
		return (1);
	}
	try
	{
		Server server(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
