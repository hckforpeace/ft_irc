#include "Client.hpp"

Client::Client(int fd):fd(fd), realname("*"), nickname("*"), username("*")
{
	this->nickname = "*";
	this->channel_counter = 0;
  this->connected = false;
}

Client::~Client()
{
    std::cout << "Client with fd: " << fd << " destroyed" << std::endl;
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

void	Client::setNickname(std::string nick)
{
	this->nickname = nick;
}

std::string	Client::getNickname()
{
	return (this->nickname);
}


std::string	Client::getUsername()
{
	return (this->username);
}

void Client::setUsername(std::string str)
{
	this->username = str;
}

void	Client::joinChanCounter(void)
{
	this->channel_counter++;
}

void  Client::setRealname(std::string str)
{
  this->realname = str;
}

std::string Client::getRealname()
{
  return (this->realname);
}


bool  Client::isRegistered()
{
  return (this->isConnected() && this->username.compare("*") && this->realname.compare("*") && this->nickname.compare("*"));
}
