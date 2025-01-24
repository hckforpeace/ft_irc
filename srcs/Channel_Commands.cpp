#include "Server.hpp"

void	Server::invite(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(cmd[0]), client->getFd()));
	if (!findClient(cmd[1]))
		return (sendMSG(ERR_UNKNOWNUSER(cmd[1]), client->getFd()));
	if (cmd[2].at(0) != '#' || (cmd[2].at(0) == '#' && cmd[2].length() == 1))
		return (sendMSG(ERR_INVCHANNELNAME, client->getFd()));
	if (!findChannel(cmd[2].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(cmd[2].substr(1)), client->getFd()));
	if (!isOperator(client, findChannel(cmd[2].substr(1))))
		return (sendMSG(ERR_NOTOPERATOR(cmd[2].substr(1)), client->getFd()));
	Client *invited = findClient(cmd[1]);
	invited->addtoInviteChan(cmd[2].substr(1));
	sendMSG(BE_INVITED(client->getNickname(), cmd[2]), invited->getFd());
	sendMSGChan(TO_INVITE(client->getNickname(), invited->getNickname(), cmd[2]), findChannel(cmd[2].substr(1)));
}

void	Server::topic(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(cmd[0]), client->getFd()));
	if (cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd[1].length() == 1))
		return (sendMSG(ERR_INVCHANNELNAME, client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(cmd[1].substr(1)), client->getFd()));
	Channel *channel = findChannel(cmd[1].substr(1));
	if (cmd.size() == 2)
	{
		if (channel->getTopic().empty())
			return(sendMSG(EMPTY_TOPIC(channel->getName()), client->getFd()));
		return(sendMSG(SHOW_TOPIC(channel->getName(), channel->getTopic(), client->getNickname()), client->getFd()));
	}
	if (!isinChan(client, channel))
		return (sendMSG(ERR_USERNOTONCHAN(channel->getName()), client->getFd()));
	if (!isOperator(client, channel) && channel->getTopicMode())
		return (sendMSG(ERR_NOTOPERATOR(channel->getName()), client->getFd()));
	channel->setTopic(cmd[2]);
	sendMSG(CHANGE_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()), client->getFd());
}
