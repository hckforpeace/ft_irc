#include "Server.hpp"

bool Server::nickInUse(std::string nickname)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (!(*it)->getNickname().compare(nickname))
			return (true);
	}
	return (false);
}

// check if the message sent by the client is ended by '\r\n' 
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

// Send a messahe to all clients in a channel
void Server::sendMSGChan(std::string message, Channel *channel)
{
	std::vector<Client *> client = channel->getClients();
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); it++)
	{
		this->sendMSG(message, (*it)->getFd());
	}
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		this->sendMSG(message, (*it)->getFd());
	}
}

bool Server::isinChan(Client *client, Channel *channel)
{
	std::vector<Client *> chan_client = channel->getClients();
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = chan_client.begin(); it != chan_client.end(); it++)
	{
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	}
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	}
	return (false);
}

bool Server::isOperator(Client *client, Channel *channel)
{
	std::vector<Client *> operators = channel->getOperators();

	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	}
	return (false);
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

// send a RPL to the Client in the IRC format norm
void Server::sendMSG(std::string message, int fd)
{
	const char *buffer;
	message.append("\r\n");

	if (!isOpenedSock(fd))
		return;
	std::cout << RED "[SERVER] <" << fd << "> => " RESET << GREEN << message << RESET << std::endl;
	buffer = message.c_str();
	if (send(fd, buffer, message.length(), 0) == -1)
		std::cout << RED << "send() failed" << RESET << std::endl;
}

Channel *Server::findChannel(std::string channelname)
{
	for (std::vector<Channel *>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
	{
		if (!(*it)->getName().compare(channelname))
			return (*it);
	}
	return (NULL);
}

void Server::sendToChannel(std::string message, Channel *chan, Client *client)
{
	std::vector<Client *> cli = chan->getClients();
	std::vector<Client *> operators = chan->getOperators();

	for (std::vector<Client *>::iterator it = cli.begin(); it != cli.end(); it++)
	{
		if ((*it) != client)
			this->sendMSG(":" + client->getNickname() + " PRIVMSG " + "#" + chan->getName() + " :" + message, (*it)->getFd());
	}
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if ((*it) != client)
			this->sendMSG(":" + client->getNickname() + " PRIVMSG " + "#" + chan->getName() + " :" + message, (*it)->getFd());
	}
}

void    	Server::sendChanExptcli(Channel *chan, std::string msg, Client *client)
{
	std::vector<Client *> cli = chan->getClients();
	std::vector<Client *> operators = chan->getOperators();

	for (std::vector<Client *>::iterator it = cli.begin(); it != cli.end(); it++)
	{
		if ((*it) != client)
			this->sendMSG(msg, (*it)->getFd());
	}
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if ((*it) != client)
			this->sendMSG(msg, (*it)->getFd());
	}
}	


void Server::send_to_all_client(std::string message)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
		sendMSG(message, (*it)->getFd());
}

void Server::removeChan(Channel *channel)
{
	this->Channels.erase(this->getChannelIt(channel->getName()));
	delete channel;
}

void Server::removeClient(Client *client)
{
	this->Clients.erase(getClientIt(client->getFd()));
	delete client;
}

// generates a newNickName in case a client has by default a nickname already in use
std::string Server::generateNick(std::string base)
{
	std::string newNick;
	std::stringstream str;

	for (int i = 1; i < 2000; i++)
	{
		str << i;
		newNick = base + str.str();
		if (!nickInUse(newNick))
		{
			return (newNick);
		}
		str.str("");
		str.clear();
		newNick = base;
	}
	return ("Error");
}

// verify if the clietn is well authenticated and if so sends the welcome message ONLY ONCE
void Server::check_connection()
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (!(*it)->isAuthenticated() && (*it)->isRegistered())
		{
			(*it)->setAuthentication();
			(*it)->setFirstConnection();
			sendMSG(WLC((*it)->getUsername(), (*it)->getNickname()), (*it)->getFd());
		}
	}
}

// chekc if a socket is still opened
bool Server::isOpenedSock(int socket)
{
	struct stat statbuf;

	// Call fstat on the socket file descriptor
	if (fstat(socket, &statbuf) == -1)
		return (false); // If fstat fails, the socket is likely closed or invalid
	return (true);
}

// destroys a channel if it only contains the client that will be destroyed and then destroyes the channel
void Server::destroy_cli_chan(Client *client)
{
	for (std::vector<Channel *>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
	{
		if (this->isinChan(client, (*it)))
		{
			if ((*it)->isOperator(client))
				(*it)->removeOperator(client);
			else if ((*it)->isInChannel(client))
				(*it)->removeClient(client);
			if ((*it)->getClientNb() == 0)
			{
				this->removeChan((*it)); // problem here
				break;
			}
		}
	}
	removeClient(client);
}

std::string Server::genWhoisRpl(std::string client, std::string nick)
{
	std::string reponse = RPL_WHOISCHANNELS(client, nick);
	bool first = true;

	for (std::vector<Channel *>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
	{
		if (first)
		{
			reponse += "#" + (*it)->getName();
			first = false;
		}
		else
			reponse += " #" + (*it)->getName();
	}
	return (reponse);
}
