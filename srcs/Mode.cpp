#include "Server.hpp"

void	Server::mode(std::vector<std::string> cmd, Client *client)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	Channel *channel = findChannel(cmd[1].substr(1));
	if (!isOperator(client, channel))
		return (sendMSG(ERR_NOTOPERATOR(client->getNickname(), channel->getName()), client->getFd()));
	if (cmd[2].length() != 2 || (cmd[2].at(0) != '-' && cmd[2].at(0) != '+'))
		return (sendMSG(ERR_BADMODESYNTAX, client->getFd()));
	if (cmd[2].at(1) != 'i' && cmd[2].at(1) != 't' && cmd[2].at(1) != 'k' && cmd[2].at(1) != 'o' && cmd[2].at(1) != 'l')
		return (sendMSG(ERR_BADMODE, client->getFd()));
	if (!cmd[2].compare("+i") || !cmd[2].compare("-i")) // invite mode
		inviteMode(cmd[2], client, channel);
	if (!cmd[2].compare("+t") || !cmd[2].compare("-t")) // topic mode
		topicMode(cmd[2], client, channel);
	if (!cmd[2].compare("+k") && cmd[3].empty())
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (!cmd[2].compare("+k") && !cmd[3].empty()) // key mode
		keyModeOn(cmd[2], cmd[3], client, channel);
	if (!cmd[2].compare("-k")) // key mode off
		keyModeOff(cmd[2], client, channel);
	if (!cmd[2].compare("+o") || !cmd[2].compare("-o")) // operator mode
		operatorMode(cmd[2], cmd[3], client, channel);
	if (!cmd[2].compare("+l") || !cmd[2].compare("-l")) // limit mode on
		limitModeOn(cmd[2], client, channel);
}

void Server::inviteMode(std::string mode, Client *client, Channel *channel)
{
	if (mode.at(0) == '+')
	{
		channel->setInvite(true);
		sendMSGChan(MODE_SET(channel->getName(), client->getNickname(), "i"), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setInvite(false);
		sendMSGChan(MODE_UNSET(channel->getName(), client->getNickname(), "i"), channel);
	}
}

void	Server::topicMode(std::string mode, Client *client, Channel *channel)
{
	if (mode.at(0) == '+')
	{
		channel->setTopicMode(true);
		sendMSGChan(MODE_SET(channel->getName(), client->getNickname(), "t"), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setTopicMode(false);
		sendMSGChan(MODE_UNSET(channel->getName(), client->getNickname(), "t"), channel);
	}
}

void	Server::keyModeOn(std::string mode, std::string password, Client *client, Channel *channel)
{
	channel->setKeyMode(true);
	sendMSGChan(MODE_SET(channel->getName(), client->getNickname(), "k " + password), channel);
}

void	Server::keyModeOff(std::string mode, Client *client, Channel *channel)
{
	channel->setKeyMode(false);
	sendMSGChan(MODE_UNSET(channel->getName(), client->getNickname(), "k " + channel->getPassword()), channel);
}

void	Server::operatorMode(std::string mode, std::string new_operator, Client *client, Channel *channel)
{

}

void	Server::limitModeOn(std::string limit, Client *client, Channel *channel)
{

}

void	Server::limitModeOff(Client *client, Channel *channel)
{

}
