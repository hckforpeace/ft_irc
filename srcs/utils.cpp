#include <Server.hpp>
#include <cstddef>
#include <vector>

bool Server::nickInUse(std::string nickname)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (!(*it)->getNickname().compare(nickname))
			return (true);
	}
	return (false);
}

bool Server::isCRLF(std::string str, Client *client)
{
	std::size_t pos = str.find("\r\n");
	std::string sub;

	if (pos == std::string::npos)
	{
		client->setMessage(client->getMessage() + str);
		return (false);
	}
	sub = str.substr(0, str.length() - 2);
	client->setMessage(client->getMessage() + sub);
	return (true);
}

Client *Server::findClient(std::string nickname)
{
	for (std::vector<Client *>::iterator it = this->Clients.begin(); it != this->Clients.end(); it++)
	{
		if (!(*it)->getNickname().compare(nickname))
			return (*it);
	}
	return (NULL);
}

void Server::sendMSG(std::string message, int fd)
{
	const char *buffer;
	message.append("\r\n");

	buffer = message.c_str();
	if (send(fd, buffer, message.length(), 0) == -1)
		std::cout << RED << "send() failed" << RESET << std::endl;
}

void Server::sendMSGChan(std::string message, Channel *channel)
{
	std::vector<Client *> client = channel->getClients();
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); it++)
		this->sendMSG(message, (*it)->getFd());
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
		this->sendMSG(message, (*it)->getFd());
}

void Server::sendToChannel(std::string message, std::string nickname, Channel *channel)
{
	std::vector<Client *> client = channel->getClients();
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); it++)
	{
		this->sendMSG("<" + nickname + ":" + BLU + "#" + channel->getName() + RESET + "> " + message, (*it)->getFd());
	}
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		this->sendMSG("<" + nickname + ":" + BLU + "#" + channel->getName() + RESET + "> " + message, (*it)->getFd());
	}
}

bool Server::isinChan(Client *client, Channel *channel)
{
	std::vector<Client *> chan_client = channel->getClients();
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = chan_client.begin(); it != chan_client.end(); it++)
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	return (false);
}

bool Server::isOperator(Client *client, Channel *channel)
{
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	return (false);
}

bool	setOperator(Client *client, Channel *channel)
{

}

/*=========================== GETTERS ======================================*/
Channel* Server::findChannel(std::string channel_name)
{
	for (std::vector<Channel *>::iterator it = Channels.begin(); it != Channels.end(); it++)
		if (!(*it)->getName().compare(channel_name))
			return (*it);
	return (NULL);
}
