#include "Server.hpp"

// Program Parser
void Server::parse_args(char *port, char *password)
{
	std::string s_password(password);
	std::stringstream ss(port);
	int port_nb;

	ss >> port_nb;
	if (ss.fail())
		throw std::runtime_error("Port is not a number");
	if (port_nb > 65535)
		throw std::runtime_error("Port must be between 0 and 65535");
	this->_port = port_nb;
	this->_password = s_password; // password has a length limit?
}

// splits on space
std::vector<std::string> Server::split_buffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream ss(str);
	std::string token;

	while (ss >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return (vec);
}

// splits on '\n'
std::vector<std::string> Server::split_line_buffer(const char *sentence)
{
	std::stringstream ss(sentence);
	std::vector<std::string> message;
	std::string line;
	if (sentence != NULL)
	{
		while (std::getline(ss, line, '\n'))
			message.push_back(line);
	}
	return (message);
}