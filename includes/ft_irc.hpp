#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>
# include <string>
# include <stdexcept>

// Colors
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"

class Server {
	private:
		std::string	port;
		std::string	password;

	public:
		Server(void) {};
		void	parse_args(char *port, char *password);
};

#endif
