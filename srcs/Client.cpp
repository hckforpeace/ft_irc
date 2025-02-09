#include "Server.hpp"

Client::Client(int fd) : fd(fd), realname("*"), nickname("*"), username("*"), password_inserted(false), first_connection(true), privmsg_param("")
{
	this->nickname = "*";
	this->channel_counter = 0;
	this->authenticated = false;
}

Client::~Client()
{
    std::cout << "Client with fd: " << fd << " destroyed" << std::endl;
    if (close(fd) == -1)
		  std::cerr << "failed to close fd: " << fd << std::endl;
}

/* ************************************************************************** */
/*                                  Getters                                   */
/* ************************************************************************** */
int Client::getFd()
{
	return (this->fd);
}

std::string &Client::getMessage()
{
	return (this->message);
}


bool Client::isAuthenticated()
{
	return (this->authenticated);
}

std::string Client::getNickname()
{
	return (this->nickname);
}

std::string Client::getUsername()
{
	return (this->username);
}

std::string Client::getHostname(void)
{
	std::string hostname = this->nickname + "!" + this->username;
	return (hostname);
}

std::string Client::getRealname()
{
	return (this->realname);
}

int Client::getChanCounter(void)
{
	return (this->channel_counter);
}

std::string  Client::getCmdParams()
{
	return (this->privmsg_param);
}

bool Client::getPassstatus()
{
	return (this->password_inserted);
}

bool Client::firstConnection(void)
{
	return (this->first_connection);
}

/*========================== SETTERS ===========================*/

void Client::setPasswordInserted()
{
	this->password_inserted = true;
}

void Client::setNickname(std::string nick)
{
	this->nickname = nick;
}

void Client::setMessage(std::string str)
{
	this->message = str;
}

void Client::setAuthentication(void)
{
	this->authenticated = true;
}

void Client::setUsername(std::string str)
{
	this->username = str;
}

void Client::increaseChanCounter(void)
{
	this->channel_counter++;
}

void Client::decreaseChanCounter(void)
{
	this->channel_counter--;
}

void Client::setRealname(std::string str)
{
	this->realname = str;
}

void Client::setFirstConnection()
{
	this->first_connection = false;
}

/* ************************************************************************** */
/*                             Public Member Methods                          */
/* ************************************************************************** */

// stores the parameter of PRIVMSG command
void Client::setPrivmsgParam(std::string msg, bool flag)
{
	size_t i = 0;
	std::string param;
	while (i < msg.size() && msg[i] == ' ')
		i++;
	while (i < msg.size() && msg[i] != ' ')
		i++;
	while (i < msg.size() && msg[i] == ' ')
		i++;
	if (flag)
	{
		param = msg.substr(i);
		if (param.find('\r') != std::string::npos)
			param = param.erase(param.find('\r'), 2); // Remove the \r\n
		this->privmsg_param = param;
		return ;
	}
	while (i < msg.size() && msg[i] != ' ')
		i++;
	while (i < msg.size() && msg[i] == ' ')
		i++;
	param = msg.substr(i);
	if (param.find('\r') != std::string::npos)
		param = param.erase(param.find('\r'), 2); // Remove the \r\n
	this->privmsg_param = param; 
}

// stores the parameter of QUIT command
void	Client::setQuitParam(std::string msg)
{
	size_t i = 0;
	std::string param;

	while (i < msg.size() && msg[i] == ' ')
	i++;
	while (i < msg.size() && msg[i] != ' ')
	i++;
	param = msg.substr(i + 1);
	// Remove the \r\n
	if (param.find('\r') != std::string::npos)
		param = param.erase(param.find('\r'), 2);
	this->privmsg_param = param; 
}

// A Client is registered if he has set succesffuly the password, if nickname is set and if USER is set
bool  Client::isRegistered()
{
	return (this->getPassstatus() && this->username.compare("*") && this->realname.compare("*") && this->nickname.compare("*"));
}

void Client::addtoInviteChan(std::string channel)
{
	std::vector<std::string> vec;

	vec.push_back(channel);
	this->invited_to_chan.clear();
	this->invited_to_chan = vec;
}

bool Client::isInvited(std::string channel)
{
	if (invited_to_chan.empty())
		return (false);
	for (std::vector<std::string>::iterator it = invited_to_chan.begin(); it <= invited_to_chan.end(); it++)
		if (!(*it).compare(channel))
			return (true);
	return (false);
}
