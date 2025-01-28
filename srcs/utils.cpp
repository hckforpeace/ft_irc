#include "Channel.hpp"
#include "Client.hpp"
#include "Colors.hpp"
#include <Server.hpp>
#include <cstddef>
#include <vector>

bool	Server::nickInUse(std::string nickname)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (!(*it)->getNickname().compare(nickname))
			return (true);
	}
	return (false);
}


bool	Server::isCRLF(std::string str, Client *client)
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


Client*  Server::findClient(std::string nickname)
{
  for (std::vector<Client*>::iterator it = this->Clients.begin(); it != this->Clients.end(); it++)
  {
    if (!(*it)->getNickname().compare(nickname))
      return (*it);
  }
  return (NULL);
}


void    Server::sendMSG(std::string message, int fd)
{
  const char *buffer;
  message.append("\r\n");

  buffer = message.c_str();
  if (send(fd, buffer, message.length(), 0) == -1)
    std::cout << RED << "send() failed" << RESET << std::endl;
}


/* void    Server::sendRPL(std::string cmd, int fd)
{

} */

Channel*  Server::findChannel(std::string channelname)
{  
  for (std::vector<Channel*>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
  {
    if (!(*it)->getName().compare(channelname))
      return (*it);
  }
  return (NULL);
}

void  Server::sendToChannel(std::string message, std::string nickname, Channel *chan, Client *client)
{ 
  std::vector<Client*> cli = chan->getClients();
  std::vector<Client*> operators = chan->getOperators();

  std::string is_op = "";
  if (chan->isOperator(client))
      is_op = "@";
  std::cout << "is he op ?: " << is_op << std::endl;
  for (std::vector<Client*>::iterator it = cli.begin(); it != cli.end(); it++)
  {
    this->sendMSG("<" + is_op + nickname + ":" + BLU + "#" + chan->getName() + RESET + "> " + message, (*it)->getFd());
  }
  for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); it++)
  {
    this->sendMSG("<" + is_op + nickname + ":" + BLU + "#" + chan->getName() + RESET + "> " + message, (*it)->getFd());  
  }
}

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