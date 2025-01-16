#include "ft_irc.hpp"

int	main(int argc, char **argv)
{
	Server serv;

	if (argc != 3)
	{
		std::cout << RED << "Wrong number of arguments. Run as ./ircserver <port> <password>" << RESET << std::endl;
		return (1);
	}
	try
	{
		serv.parse_args(argv[1], argv[2]);
		// launch server here
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
