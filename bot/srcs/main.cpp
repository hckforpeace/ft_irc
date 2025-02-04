#include "bot.hpp"
#include "Colors.hpp"
#include <sstream>

int main (int argc, char *argv[])
{

	std::vector<std::string>	forbidden_words;
	std::stringstream 			str;
	int							port;

	if (argc < 6)
		std::cout << RED << "WRONG INPUT EXPECTED" << RESET GREEN << " => ./bot port_number ip_addr nickname channel_name forbiden_words.."  << RESET << std::endl;  
	else
	{
		for (int i = 5; i < argc; i++)
			forbidden_words.push_back(argv[i]);
		str << argv[1];
		str >> port;
		Bot b(port, argv[2], argv[3], argv[4], forbidden_words);

	}

	return (0);
}