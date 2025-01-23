#include "Server.hpp"

void	Server::mode(std::vector<std::string> cmd, Client *client)
{
	if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(cmd[0]), client->getFd()));
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
		return (sendMSG(ERR_INVCHANNELNAME, client->getFd()));
	if (!getChannel(cmd[1].substr(1)))
		return (sendMSG(ERR_NOSUCHCHANNEL(cmd[1].substr(1)), client->getFd()));
	Channel *channel = getChannel(cmd[1].substr(1));
	if (!isOperator(client, channel))
		return (sendMSG(ERR_NOTOPERATOR(channel->getName()), client->getFd()));
	if (cmd[2].length() != 2 || (cmd[2].at(0) != '-' && cmd[2].at(0) != '+'))
		return (sendMSG(ERR_BADMODESYNTAX, client->getFd()));
	if (cmd[2].at(1) != 'i' && cmd[2].at(1) != 't' && cmd[2].at(1) != 'k' && cmd[2].at(1) != 'o' && cmd[2].at(1) != 'l')
		return (sendMSG(ERR_BADMODE, client->getFd()));
	if (!cmd[2].compare("+i"))
		channel->invite();
}
