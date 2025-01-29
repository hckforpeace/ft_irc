#include "Server.hpp"

void	Server::invite(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].empty() || !findClient(cmd[1]))
		return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), cmd[1]), client->getFd()));
	if (cmd[2].at(0) != '#' || (cmd[2].at(0) == '#' && cmd[2].length() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[2]), client->getFd()));
	if (!findChannel(cmd[2].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[2]), client->getFd()));
	if (!isOperator(client, findChannel(cmd[2].substr(1))))
		return (sendMSG(ERR_NOTOPERATOR(client->getNickname(), cmd[2].substr(1)), client->getFd()));
	Client *invited = findClient(cmd[1]);
	invited->addtoInviteChan(cmd[2].substr(1));
	sendMSG(BE_INVITED(client->getNickname(), cmd[2]), invited->getFd());
	sendMSGChan(TO_INVITE(client->getNickname(), invited->getNickname(), cmd[2]), findChannel(cmd[2].substr(1)));
}

void	Server::topic(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd[1].length() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	Channel *channel = findChannel(cmd[1].substr(1));
	if (cmd.size() == 2)
	{
		if (channel->getTopic().empty())
			return(sendMSG(EMPTY_TOPIC(channel->getName()), client->getFd()));
		return(sendMSG(SHOW_TOPIC(channel->getName(), channel->getTopic(), client->getNickname()), client->getFd()));
	}
	if (!isinChan(client, channel))
		return (sendMSG(ERR_USERNOTINCHAN1(client->getNickname(), channel->getName()), client->getFd()));
	if (!isOperator(client, channel) && channel->getTopicMode())
		return (sendMSG(ERR_NOTOPERATOR(client->getNickname(), channel->getName()), client->getFd()));
	channel->setTopic(cmd[2]);
	sendMSG(CHANGE_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()), client->getFd());
}

void	Server::part(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd[1].length() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	Channel *channel = findChannel(cmd[1].substr(1));
	if (!isinChan(client, channel))
		return (sendMSG(ERR_USERNOTINCHAN1(client->getNickname(), channel->getName()), client->getFd()));
	sendMSGChan(RPL_PART(client->getNickname(), client->getUsername(), channel->getName()), channel);
	if (isOperator(client, channel))
		channel->removeOperator(client);
	else
		channel->removeClient(client);
	if (channel->getClientNb() == 0)
		removeChan(channel);
}

void	Server::kick(Client *client, std::vector<std::string> cmd)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd[1].length() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	Client *kicked_client = findClient(cmd[2]);
	Channel *channel = findChannel(cmd[1].substr(1));
	if (!kicked_client)
		return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), cmd[1]), client->getFd()));
	if (!isinChan(client, channel))
		return (sendMSG(ERR_USERNOTINCHAN2(client->getNickname(), channel->getName()), client->getFd()));
	sendMSGChan(RPL_KICK(kicked_client->getNickname(), kicked_client->getUsername(), channel->getName()), channel);
	if (isOperator(kicked_client, channel))
		channel->removeOperator(kicked_client);
	else
		channel->removeClient(kicked_client);
	if (channel->getClientNb() == 0)
		removeChan(channel);
}
