#include "ft_irc.hpp"

void	Server::parse_args(char *port, char *password)
{
	std::string	s_port(port);
	std::string s_password(password);

	this->port = s_port;
	this->password = s_password;
}
