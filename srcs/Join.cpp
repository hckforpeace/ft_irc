#include "Server.hpp"

void Server::join(Client *client, std::vector<std::string> cmd)
{
	int flag = 0;
	std::string	password;

	if (cmd.size() < 3)
		password = "";
	else
		password = cmd[2];
	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd[1].length() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	for (int i = 0; i < this->Channels.size(); i++)
	{
		if (this->Channels[i]->getName() == cmd[1].substr(1))
		{
			enterChannel(this->Channels[i], client, password);
			flag = 1;
			break ;
		}
	}
	if (!flag)
		createChannel(cmd[1].substr(1), client);
}
void Server::createChannel(std::string name, Client *client)
{
	if (client->getChanCounter() >= 10) // limit of connected channels per client
		return (sendMSG(ERR_TOOMANYCHANNELS(client->getNickname(), name), client->getFd()));
	Channel *new_channel = new Channel(name);

	new_channel->setName(name);
	new_channel->add_operator(client);
	this->Channels.push_back(new_channel);
	client->setChanCounter();
	sendMSG(RPL_JOIN(client->getHostname(), new_channel->getName()) + \
		RPL_NAMES(client->getNickname(), new_channel->getName(), new_channel->getClientLst()) + \
		RPL_ENDOFNAMES(client->getNickname(),new_channel->getName()), client->getFd());
}

void Server::enterChannel(Channel *channel, Client *client, std::string password)
{
	if (isinChan(client, channel)) // if the client is already on the channel it can't reconnect
		return ;
	if (client->getChanCounter() >= 10) // limit of connected channels per client
		return (sendMSG(ERR_TOOMANYCHANNELS(client->getNickname(), channel->getName()), client->getFd()));
	if (channel->getTotalClient() >= channel->getLimit()) // if limit of connected clients in the channel
		return (sendMSG(ERR_CHANNELISFULL(client->getNickname(), channel->getName()), client->getFd()));
	if (channel->isInviteOnly() && !client->isInvited(channel->getName())) // if the channel is invite_only and the client is not invited
		return (sendMSG(INVITE_ONLY(client->getNickname(), channel->getName()), client->getFd()));
	std::cout << "channel PASS->" << channel->getPassword() << std::endl;
	std::cout << "client PASS->" << password << std::endl;
	if (channel->getKeyMode() && password.compare(channel->getPassword())) // if the channel is channel-protected and the password is wrong
		return (sendMSG(CHAN_PASS(client->getNickname(), channel->getName()), client->getFd()));
	sendMSGChan(RPL_JOIN(client->getHostname(), channel->getName()), channel);
	channel->add_client(client);
	client->setChanCounter();
	if (channel->getTopic().empty())
		sendMSG(RPL_JOIN(client->getHostname(), channel->getName()) + \
			RPL_NAMES(client->getNickname(), channel->getName(), channel->getClientLst()) + \
			RPL_ENDOFNAMES(client->getNickname(),channel->getName()), client->getFd());
	else
		sendMSG(RPL_JOIN(client->getHostname(), channel->getName()) + \
			RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()) + \
			RPL_NAMES(client->getNickname(), channel->getName(), channel->getClientLst()) + \
			RPL_ENDOFNAMES(client->getNickname(),channel->getName()), client->getFd());
}
