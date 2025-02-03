#include "Channel.hpp"
#include "Client.hpp"
#include "Colors.hpp"
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
		if (!(*it)->getNickname().compare(client->getNickname()))
			return (true);
	return (false);
}

/*======================================== GETTERS ===============================================*/

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

  if (!isOpenedSock(fd))
    return ;
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

void Server::sendToChannel(std::string message, std::string nickname, Channel *chan, Client *client)
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
		// this->sendMSG("<" + is_op + nickname + ":" + BLU + "#" + chan->getName() + RESET + "> " + message, (*it)->getFd());
		if ((*it) != client)
			this->sendMSG(":" + client->getNickname() + " PRIVMSG " + "#" + chan->getName() + " :" + message, (*it)->getFd());
	}
}

void Server::send_to_all_client(std::string message)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
		sendMSG(message, (*it)->getFd());
}

void Server::removeChan(Channel *channel)
{
	// for (std::vector<Channel *>::iterator it = Channels.begin(); it != Channels.end(); it++)
	// {
	// 	if (!(*it)->getName().compare(channel->getName()))
	// 	{
	// 		Channels.erase(it);
    // 		// delete channel;
	// 		break ;
	// 	}
	// }
	std::vector<Channel *>::iterator it = this->getChannelIt(channel->getName());
	std::cout << "channel name form operator: " << (*it)->getName() << std::endl;
	this->Channels.erase(this->getChannelIt(channel->getName()));
	std::cout << "LENGTH OF THE CHANNEL VECTOR IS: " << Channels.size() << std::endl;
	delete channel;
	std::cout << "Number of Channels on the server down to: " << RED << Channels.size() << RESET << std::endl;

}

void	Server::removeClient(Client *client)
{
	this->Clients.erase(getClientIt(client->getFd()));
	delete client;
	std::cout << "Number of Clients on the server down to: " << RED << Clients.size() << RESET << std::endl;
}

// void		Server::removeClient(Client *client, Channel *channel)
// {
// 	std::vector<Client*> clients = channel->getClients();
// 	std::vector<Client*> operators = channel->getOperators();
// 	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
//   	{
//     	if (!client->getNickname().compare((*it)->getNickname()))

//   	}
//   	for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); it++)
//   	{
//     	this->sendMSG("<" + is_op + nickname + ":" + BLU + "#" + chan->getName() + RESET + "> " + message, (*it)->getFd());
//   	}
// }
// bool	Server::login_parse(std::vector<std::string> cmds, Client *client)
// {
//   for (int i = 0; i < cmds.size(); i++)
//   {
// 	  if ((cmds[i] == "pass" || cmds[i] == "PASS"))		
// 	  	authenticate(client, cmds[i + 1]);// authenticate  }
//   }
// }

std::string	Server::generateNick(std::string base)
{
  std::string newNick;
  std::stringstream str;

  for (int i = 1; i < 2000; i++)
  {
    str << i;
    newNick = base + str.str();
    if (!nickInUse(newNick))
      return (newNick);
    str.clear();
    str << "";
  }
  return ("Error");
}

void	Server::check_connection()
{
  for (std::vector<Client*>::iterator it = Clients.begin(); it != Clients.end(); it++)
  {
    // std::cout << "Sent WELCOME to: " << (*it)->isRegistered() << std::endl;
    if (!(*it)->isConnected() && (*it)->isRegistered())
    {
		(*it)->setConnection();
		(*it)->setFirstConnection();
		sendMSG(WLC((*it)->getUsername(), (*it)->getNickname()), (*it)->getFd());
    }
  }
}

bool    Server::isOpenedSock(int socket)
{
  struct stat statbuf;

  // Call fstat on the socket file descriptor
  if (fstat(socket, &statbuf) == -1) 
    return (false); // If fstat fails, the socket is likely closed or invalid
  return (true);
}

void    Server::destroy_cli_chan(Client *client)
{
  for (std::vector<Channel*>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
  {
    if (this->isinChan(client, (*it)))
    {
		std::cout << "***************** should delete the chan***********" << std::endl;;
		if ((*it)->isOperator(client))
		{
			std::cout << "**THE CLIENT YOU WANT TO REMOVE IS AN OPERATOR**" << std::endl;
			(*it)->removeOperator(client);
		}
		else if ((*it)->isInChannel(client))
		{
			std::cout << "**THE CLIENT YOU WANT TO REMOVE IS NOT AN OPERATOR**" << std::endl;
			(*it)->removeClient(client);
		}

		
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
  Client *target = this->findClient(nick);
  bool first = true;
  for (std::vector<Channel*>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
  {
    if ((*it)->isOperator(target))
    {
      if (first)
      {
        reponse += "@#" + (*it)->getName();
        first = false;
      }
      else
        reponse += " @#" + (*it)->getName();
    }
    else if ((*it)->isInChannel(target))
    {
      if (first)
      {
        reponse += "#" + (*it)->getName();
        first = false;
      }
      else
        reponse += " #" + (*it)->getName();
    }
  }
  return (reponse);
}
