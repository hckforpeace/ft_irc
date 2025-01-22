#include "Channel.hpp"

Channel::Channel(void)
{
	this->limit = 20; // we have to verify the real limit

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
void	Channel::add_client(Client new_client)
{

}

void	Channel::add_operator(Client new_operator)
{
	operators.push_back(new_operator);
}

/*========================== GETTERS ===============================*/
std::string	Channel::getName(void)
{
	return (this->name);
}

/*========================== SETTERS ===============================*/
void	Channel::setName(std::string name)
{
	this->name = name;
}
