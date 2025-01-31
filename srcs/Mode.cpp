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
	Channel *channel = findChannel(cmd[1].substr(1));
	if (!isOperator(client, channel))
		return (sendMSG(ERR_NOTOPERATOR(client->getNickname(), channel->getName()), client->getFd()));
	if (cmd[2].at(1) != 'i' && cmd[2].at(1) != 't' && cmd[2].at(1) != 'k' && cmd[2].at(1) != 'o' && cmd[2].at(1) != 'l')
		return (sendMSG(ERR_UNKNOWNMODE(client->getNickname(), cmd[2].substr(1, 1)), client->getFd()));
	else if (!cmd[2].compare("+i") || !cmd[2].compare("-i")) // invite mode
		inviteMode(cmd[2], client, channel);
	else if (!cmd[2].compare("+t") || !cmd[2].compare("-t")) // topic mode
		topicMode(cmd[2], client, channel);
	else if (!cmd[2].substr(0, 2).compare("-l"))
		limitMode(cmd[2], "1", client, channel);
	else if (cmd.size() < 4)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	else if (!cmd[2].compare("+k") || !cmd[2].compare("+k")) // key mode
		keyMode(cmd[2], cmd[3], client, channel);
	else if (!cmd[2].compare("+o") || !cmd[2].compare("-o")) // operator mode
		operatorMode(cmd[2], cmd[3], client, channel);
	else if (!cmd[2].compare("+l")) // limit mode
		limitMode(cmd[2], cmd[3], client, channel);
}

void Server::inviteMode(std::string mode, Client *client, Channel *channel)
{
	if (mode.at(0) == '+')
	{
		channel->setInvite(true);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+i", ""), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setInvite(false);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-i", ""), channel);
	}
}

void Server::topicMode(std::string mode, Client *client, Channel *channel)
{
	if (mode.at(0) == '+')
	{
		channel->setTopicMode(true);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+t", ""), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setTopicMode(false);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-t", ""), channel);
	}
}

void Server::keyMode(std::string mode, std::string password, Client *client, Channel *channel)
{
	if (mode.at(0) == '+')
	{
		channel->setKeyMode(true);
		channel->setPassword(password);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+k", password), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setKeyMode(false);
		channel->setPassword("");
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-k", password), channel);
	}
}

void Server::operatorMode(std::string mode, std::string new_operator, Client *client, Channel *channel)
{
	Client *op = findClient(new_operator);

	if (!op)
		return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), new_operator), client->getFd()));
	if (!isinChan(op, channel))
		return (sendMSG(ERR_USERNOTINCHAN2(client->getNickname(), channel->getName()), client->getFd()));
	if (mode.at(0) == '+')
	{
		if (!isOperator(op, channel))
		{
			channel->add_operator(op);
			channel->removeClient(op);
		}
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+o", new_operator), channel);
	}
	if (mode.at(0) == '-')
	{
		if (isOperator(op, channel))
		{
			channel->add_client(op);
			channel->removeOperator(op);
		}
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-o", new_operator), channel);
	}
}

void Server::limitMode(std::string mode, std::string limit, Client *client, Channel *channel)
{
	std::stringstream	slimit(limit);
	std::stringstream	print;
	int					nb_limit;

	slimit >> nb_limit;
	print << nb_limit;
	if (nb_limit <= 0)
		return ;
	if (nb_limit >= 512)
		return (channel->setLimit(512));
	if (mode.at(0) == '+')
	{
		channel->setLimit(nb_limit);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "+l", print.str()), channel);
	}
	if (mode.at(0) == '-')
	{
		channel->setLimit(512);
		sendMSGChan(RPL_CHANGEMODE(client->getHostname(), channel->getName(), "-l", ""), channel);
	}
}
