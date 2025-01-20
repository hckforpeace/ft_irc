#include "ft_irc.hpp"

void	Server::parse_args(char *port, char *password)
{
	std::string			s_password(password);
	std::stringstream	s_port(port);
	int					port_nb;
	
	s_port >> port_nb;
	if (s_port.fail() || !s_port.eof())
		throw std::runtime_error("Port is not a number");
	if (port_nb < 1024 || port_nb > 65535)
		throw std::runtime_error("Port must be between 0 and 65535");
	if (s_password.size() > 15)
		throw std::runtime_error("Password is too long, it's 15 characters max");
	this->port = port_nb;
	this->password = s_password;
}
