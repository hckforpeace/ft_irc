#include "Replies.hpp"
#include <Server.hpp>
#include <string>

// Commands
void Server::authenticate(Client *client, std::vector<std::string> cmd, int i)
{
	std::string err;
	int fd = client->getFd();
	if (cmd.size() < 2)
		err = ERR_NEEDMOREPARAMS(client->getNickname());
	else if (client->isConnected())
		err = ERR_ALREADYREGISTRED(client->getNickname());
	else
	{
		if (!cmd[1].compare(this->_password))
		{
			client->setPasswordInserted();
			std::cout << RED "[SERVER] => PASSWORD CORRECT !" RESET << std::endl;
			return;
		}
		else
			err = ERR_PASSWDMISMATCH(client->getNickname());
	}
	this->sendMSG(err, client->getFd());
	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &events[i]);
	destroy_cli_chan(client);
}

void Server::setNickname(Client *client, std::vector<std::string> cmd)
{
	std::string err;
	std::string old_nick;

	// if (!client->getPassstatus())
	// 	err = ERR_NOTREGISTERED(client->getNickname());
	if (cmd.size() < 2)
		err = ERR_NONICKNAMEGIVEN(client->getNickname());
	else if (client->firstConnection() && nickInUse(cmd[1]))
	{
		std::cout << "a" << std::endl;
		sendMSG(":" + cmd[1] + " NICK " + generateNick(cmd[1]), client->getFd());
		return (client->setNickname(generateNick(cmd[1])));
	}
	else if (nickInUse(cmd[1]))
	{
		std::cout << "b" << std::endl;
		sendMSG(":" + client->getNickname() + " NICK " + generateNick(cmd[1]), client->getFd());
		return (client->setNickname(generateNick(cmd[1])));
	}
	else
	{
		sendMSG(":" + client->getNickname() + " NICK " + cmd[1], client->getFd());
		return (client->setNickname(cmd[1]));
	}
	this->sendMSG(err, client->getFd());
}

void Server::setUser(Client *client, std::vector<std::string> cmd)
{
	std::string err;

	if (client->isRegistered())
		err = ERR_ALREADYREGISTRED(client->getNickname());
	if (cmd.size() < 3)
		err = ERR_NEEDMOREPARAMS(client->getNickname());
	else
	{
		client->setUsername(cmd[1]);
		client->setRealname(cmd[2]);
		return;
	}
	this->sendMSG(err, client->getFd());
}

void Server::privmsg(Client *client, std::vector<std::string> cmd)
{
	std::string err;
	Client *rcv;
	Channel *chan;
	std::string channel;
	std::string msg;

	if (!client->isRegistered())
		return (sendMSG(ERR_NOTREGISTERED(client->getNickname()), client->getFd()));
	else if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
	else if (cmd[1].at(0) == '#')
	{
		if (findChannel(cmd[1].substr(1)))
			return (sendToChannel(client->getCmdParams().substr(1), client->getNickname(), findChannel(cmd[1].substr(1)), client));
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	}
	else
	{
		if ((rcv = this->findClient(cmd[1])) != NULL)
			return (sendMSG(":" + client->getNickname() + " PRIVMSG " + cmd[1] + " " + client->getCmdParams(), rcv->getFd()));
		else
			return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), cmd[1]), client->getFd()));
	}
}

void Server::modei(Client *client, std::vector<std::string> cmd)
{
	sendMSG(":localhost MODE " + client->getNickname() + " +i", client->getFd());
}

void Server::pong(Client *client, std::vector<std::string> cmd)
{
	sendMSG("PONG " + cmd[1], client->getFd());
}

void Server::whoIs(Client *client, std::vector<std::string> cmd)
{
	sendMSG(this->genWhoisRpl(client->getNickname(), cmd[1]), client->getFd());
}

void 	Server::quit(Client *client, std::vector<std::string> cmd)
{
	std::string param = "";

	if (cmd.size() > 1)
		param = client->getCmdParams();

	for (std::vector<Channel *>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
	{
		if (this->isinChan(client, (*it)))
			this->sendMSGChan(":" + client->getHostname() + "@localhost QUIT :Quit: " + param.substr(1), (*it));
	}
}