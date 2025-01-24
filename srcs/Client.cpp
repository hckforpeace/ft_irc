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

std::string&	Client::getMessage()
{
	return (this->message);
}

bool	Client::isConnected()
{
	return (this->connected);
}

std::string	Client::getNickname()
{
	return (this->nickname);
}

std::string	Client::getUsername()
{
	return (this->username);
}

std::string Client::getRealname()
{
  return (this->realname);
}

int		Client::getChanCounter(void)
{
	return (this->channel_counter);
}

/*========================== SETTERS ===========================*/

void	Client::setNickname(std::string nick)
{
	this->nickname = nick;
}

void	Client::setMessage(std::string str)
{
	this->message = str;
}

void	Client::setConnection()
{
	this->connected = true;
}

void Client::setUsername(std::string str)
{
	this->username = str;
}

void	Client::setChanCounter(void)
{
	this->channel_counter++;
}

void  Client::setRealname(std::string str)
{
  this->realname = str;
}

/*======================== METHODS ===============================*/

bool  Client::isRegistered()
{
  return (this->isConnected() && this->username.compare("*") && this->realname.compare("*") && this->nickname.compare("*"));
}

void	Client::addtoInviteChan(std::string channel)
{
	std::vector<std::string> vec;

	vec.push_back(channel);
	this->invited_to_chan.clear();
	this->invited_to_chan = vec;
}

bool	Client::isInvited(std::string channel)
{
	if (invited_to_chan.empty())
		return (false);
	for (std::vector<std::string>::iterator it = invited_to_chan.begin(); it <= invited_to_chan.end(); it++)
		if (!(*it).compare(channel))
			return (true);
	return (false);
}
