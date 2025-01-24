#include "Server.hpp"

void	Server::mode(std::vector<std::string> cmd, Client *client)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(cmd[0]), client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
		return (sendMSG(ERR_INVCHANNELNAME, client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(cmd[1]), client->getFd()));
	Channel *channel = findChannel(cmd[1].substr(1));
	if (!isOperator(client, channel))
		return (sendMSG(ERR_NOTOPERATOR(channel->getName()), client->getFd()));
	if (cmd[2].length() != 2 || (cmd[2].at(0) != '-' && cmd[2].at(0) != '+'))
		return (sendMSG(ERR_BADMODESYNTAX, client->getFd()));
	if (cmd[2].at(1) != 'i' && cmd[2].at(1) != 't' && cmd[2].at(1) != 'k' && cmd[2].at(1) != 'o' && cmd[2].at(1) != 'l')
		return (sendMSG(ERR_BADMODE, client->getFd()));
	if (!cmd[2].compare("+i") || !cmd[2].compare("-i")) // invite mode
		inviteMode(cmd[2], client, channel);
	if (!cmd[2].compare("+t") || !cmd[2].compare("-t")) // topic mode
		topicMode(cmd[2], client, channel);
	if (!cmd[2].compare("+k") || !cmd[2].compare("-k")) // key mode
		keyMode(cmd[2], cmd[3], client, channel);
	if (!cmd[2].compare("+o") || !cmd[2].compare("-o")) // operator mode
		operatorMode(cmd[2], cmd[3], client, channel);
	if (!cmd[2].compare("+l") || !cmd[2].compare("-l")) // limit mode
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
		channel->setInvite(true);
		sendMSGChan(MODE_SET(channel->getName(), client->getNickname(), "i"), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setInvite(false);
		sendMSGChan(MODE_UNSET(channel->getName(), client->getNickname(), "i"), channel);
	}
}

void	Server::keyMode(std::string mode, std::string password, Client *client, Channel *channel)
{

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
