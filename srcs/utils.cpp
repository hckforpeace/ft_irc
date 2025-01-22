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
  message.append("\r\n");
  const char *buffer;

  buffer = message.c_str();
  if (send(fd, buffer, message.length(), 0) == -1)
    std::cout << RED << "send() failed" << RESET << std::endl;
}
