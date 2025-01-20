#include "Client.hpp"

Client::Client(int fd):fd(fd)
{

}

Client::~Client()
{
    std::cout << "Client with fd" << fd << " destroyed" << std::endl;
    if (close(fd) == -1)
		std::cerr << "failed to close fd: " << fd << std::endl;
}


int	Client::getFd()
{
	return (this->fd);
}

void	Client::setMessage(std::string str)
{
	this->message = str;
}

std::string&	Client::getMessage()
{
	return (this->message);
}

void	Client::setConnection()
{
	this->connected = true;
}


bool	Client::isConnected()
{
	return (this->connected);
}
