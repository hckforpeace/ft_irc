#include "Channel.hpp"

Channel::Channel(std::string name)
{
	this->limit = 512; //default limit when is not setted by mode +l
	this->name = name;
	this->password = "";
	this->topic_name = "";
	this->invite_only = false;
}

Channel::~Channel()
{
	//for (std::vector<Client *>::iterator it = chan_clients.begin(); it != chan_clients.end(); it++)
	//	delete *it;
	//for (std::vector<Client *>::iterator it1 = operators.begin(); it1 != operators.end(); it1++)
	//	delete *it1;
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

/*========================== GETTERS ===============================*/
std::vector<Client *> Channel::getClients(void)
{
	return (this->chan_clients);
}

std::vector<Client *> Channel::getOperators(void)
{
	return (this->operators);
}

std::string	Channel::getName(void)
{
	return (this->name);
}

int			Channel::getLimit(void)
{
	return (limit);
}

int			Channel::getTotalClient(void)
{
	int count = 0;

	for(int i = 0; i < chan_clients.size(); i++)
		count++;
	for(int i = 0; i < operators.size(); i++)
		count++;
	return (count);
}

/*========================== SETTERS ===============================*/
void	Channel::setName(std::string name)
{
	this->name = name;
}

void	Channel::setLimit(int limit)
{
	this->limit = limit;
}
