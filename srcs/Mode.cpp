#include "Server.hpp"

void Server::mode(std::vector<std::string> cmd, Client *client)
{
	if (cmd.size() < 2)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	if (findClient(cmd[1]))
		return (sendMSG("MODE " + cmd[1] + " +i", client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (!findChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	if (cmd.size() < 3)
		return;

	Channel	*channel = findChannel(cmd[1].substr(1));
	char	s, m = ' ';

	if (!isOperator(client, channel))
		return (sendMSG(ERR_NOTOPERATOR(client->getNickname(), channel->getName()), client->getFd()));
	for (std::size_t i = 0; i < cmd[2].size(); i++)
	{
		if (cmd[2].at(i) == '+' || cmd[2].at(i) == '-')
			s = cmd[2].at(i);
		else if (cmd[2].at(i) != 'i' && cmd[2].at(i) != 't' && cmd[2].at(i) != 'k' && cmd[2].at(i) != 'o' && cmd[2].at(i) != 'l')
			sendMSG(ERR_UNKNOWNMODE(client->getNickname(), cmd[2].substr(i, 1)), client->getFd());
		else
		{
			m = cmd[2].at(i);
			select_mode(s, m, cmd, client, channel);
		}
	}
}

void Server::select_mode(char sign, char mode, std::vector<std::string> cmd, Client *client, Channel *channel)
{
	if (mode == 'i') // invite mode
		inviteMode(sign, client, channel);
	else if (mode == 't') // topic mode
		topicMode(sign, client, channel);
	else if (mode == 'l' && sign == '-') // unset limit mode
		limitMode(sign, "1", client, channel);
	else if (cmd.size() < 4)
		return (sendMSG(ERR_NEEDMOREPARAMSMODE(client->getNickname(), cmd[2]), client->getFd()));
	else if (mode == 'k') // key mode
		keyMode(sign, cmd[3], client, channel);
	else if (mode == 'o') // operator mode
		operatorMode(sign, cmd[3], client, channel);
	else if (mode == 'l' && sign == '+') // set limit mode
		limitMode(sign, cmd[3], client, channel);
}

void Server::inviteMode(char sign, Client *client, Channel *channel)
{
	if (sign == '+')
	{
		channel->setInvite(true);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+i", ""), channel);
	}
	if (sign == '-')
	{
		channel->setInvite(false);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-i", ""), channel);
	}
}

void Server::topicMode(char sign, Client *client, Channel *channel)
{
	if (sign == '+')
	{
		channel->setTopicMode(true);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+t", ""), channel);
	}
	if (sign == '-')
	{
		channel->setTopicMode(false);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-t", ""), channel);
	}
}

void Server::keyMode(char sign, std::string password, Client *client, Channel *channel)
{
	if (sign == '+')
	{
		channel->setKeyMode(true);
		channel->setPassword(password);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+k", password), channel);
	}
	if (sign == '-')
	{
		channel->setKeyMode(false);
		channel->setPassword("");
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-k", password), channel);
	}
}

void Server::operatorMode(char sign, std::string new_operator, Client *client, Channel *channel)
{
	Client *op = findClient(new_operator);

	if (!op)
		return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), new_operator), client->getFd()));
	if (!isinChan(op, channel))
		return (sendMSG(ERR_USERNOTINCHAN2(client->getNickname(), channel->getName()), client->getFd()));
	if (sign == '+')
	{
		if (!isOperator(op, channel))
		{
			channel->add_operator(op);
			channel->removeClient(op);
		}
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+o", new_operator), channel);
	}
	if (sign == '-')
	{
		if (isOperator(op, channel))
		{
			channel->add_client(op);
			channel->removeOperator(op);
		}
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-o", new_operator), channel);
	}
}

void Server::limitMode(char sign, std::string limit, Client *client, Channel *channel)
{
	std::stringstream slimit(limit);
	std::stringstream print;
	int nb_limit;

	slimit >> nb_limit;
	print << nb_limit;
	if (nb_limit <= 0)
		return;
	if (nb_limit >= 512)
		return (channel->setLimit(512));
	if (sign == '+')
	{
		channel->setLimit(nb_limit);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+l", print.str()), channel);
	}
	if (sign == '-')
	{
		channel->setLimit(512);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-l", ""), channel);
	}
}
