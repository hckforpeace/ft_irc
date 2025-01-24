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
	Client *invited = findClient(cmd[1]);
	invited->addtoInviteChan(cmd[2].substr(1));
	sendMSG(BE_INVITED(client->getNickname(), cmd[2]), invited->getFd());
	sendMSGChan(TO_INVITE(client->getNickname(), invited->getNickname(), cmd[2]), findChannel(cmd[2].substr(1)));
}
