#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string name)
{
	this->limit = 20; // we have to verify the real limit
	this->name = name;
	
}

Channel::~Channel()
{

}

Channel::Channel(Channel const &src)
{

}

// Channel &Channel::operator=(Channel const &src)
// {

// }

/*=========================== METHODS ==============================*/
void	Channel::add_client(Client *new_client)
{
	chan_clients.push_back(new_client);
}

void	Channel::add_operator(Client *new_operator)
{
	operators.push_back(new_operator);
}

bool  Channel::isInChannel(Client *client)
{
  for (std::vector<Client*>::iterator it = this->chan_clients.begin(); it != this->chan_clients.end(); it++)
  {
    if ((*it) == client)
      return (true);
  }
  for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
  {
    if ((*it) == client)
      return (true);
  }
  return (false);
}


bool  Channel::isOperator(Client *client)
{
  if (!isInChannel(client))
    return (false);

  for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
  {
    if ((*it) == client)
      return (true);
  }
  return (false);

}
/*========================== GETTERS ===============================*/
std::string	Channel::getName(void)
{
	return (this->name);
}

std::vector<Client*>     Channel::getClients()
{
  return (this->chan_clients);
}

std::vector<Client*>     Channel::getOperators()
{
  return (this->operators);
}
/*========================== SETTERS ===============================*/
void	Channel::setName(std::string name)
{
	this->name = name;
}


