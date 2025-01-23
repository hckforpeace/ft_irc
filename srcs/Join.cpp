#include "Server.hpp"

void Server::join(std::vector<std::string> cmd, Client *client)
{
	int flag = 0;

	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(cmd[0]), client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
		return (sendMSG(ERR_INVCHANNELNAME, client->getFd()));
	for (int i = 0; i < this->Channels.size(); i++)
	{
		if (this->Channels[i]->getName() == cmd[1].substr(1))
		{
			enterChannel(this->Channels[i], client);
			flag = 1;
			break ;
		}
	}
	if (!flag)
		createChannel(cmd[1].substr(1), client);
}

void Server::createChannel(std::string name, Client *client)
{
	if (client->getChanCounter() >= 10)
		return (sendMSG(ERR_TOOMANYCHANNELS(client->getNickname()), client->getFd()));

	Channel *new_channel = new Channel(name);

	new_channel->setName(name);
	new_channel->add_operator(client);
	this->Channels.push_back(new_channel);
	client->setChanCounter();
	sendMSGChan(CHAN_WELC(client->getNickname(), name), new_channel);
}

void Server::enterChannel(Channel *channel, Client *client)
{
	if (client->getChanCounter() >= 10)
		return (sendMSG(ERR_TOOMANYCHANNELS(client->getNickname()), client->getFd()));
	if (channel->getTotalClient() >= channel->getLimit())
		return (sendMSG(ERR_CHANNELISFULL(channel->getName()), client->getFd()));
	if (isinChan(client, channel))
		return (sendMSG(ERR_USERONCHANNEL(client->getNickname(), channel->getName()), client->getFd()));
	channel->add_client(client);
	client->setChanCounter();
	sendMSGChan(CHAN_WELC(client->getNickname(), channel->getName()), channel);
}
