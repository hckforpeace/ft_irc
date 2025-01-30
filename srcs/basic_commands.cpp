#include "Replies.hpp"
#include <Server.hpp>
#include <string>

// Commands
void	Server::authenticate(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

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
			return ;
		}
		else
			err = ERR_PASSWDMISMATCH(client->getNickname());
	}

	this->sendMSG(err, client->getFd()); 
  destroy_cli_chan(client);
}

void	Server::setNickname(Client *client, std::vector<std::string> cmd)
{
	std::string		err;
	std::string 	old_nick;

	if (!client->getPassstatus())
		err = ERR_NOTREGISTERED(client->getNickname());
	else if (cmd.size() < 2)
		err = ERR_NEEDMOREPARAMS(client->getNickname());
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
	// else
	// {
	// 	if (client->getNickname().compare(""))
	// 		old_nick = client->getNickname();
	// 	else
	// 		old_nick = cmd[1];
	// 	if (nickInUse(cmd[1]))
	// 	{
	// 		old_nick = cmd[1];
	// 		cmd[1] = generateNick(cmd[1]);
	// 	}
	// 	std::string temp = cmd[1];
	// 	if (temp.at(0) == ':' || temp.at(0) == '#')
	// 		err = ERR_ERRONEUSNICKNAME(cmd[1]);
	// 	else
    // 	{
	// 		client->setNickname(cmd[1]);
 	// 		sendMSG(":" + old_nick + " NICK " + cmd[1], client->getFd());
 	// 		// sendMSG(":" + old_nick + "!" + client->getUsername() + "@localhost " + " NICK " + cmd[1], client->getFd());
	// 		return ;
	// 	}
	// }
  	// std::cout << "SetNickname error message sends: " << err << std::endl;
}

void	Server::setUser(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	if (client->isRegistered())
		err = ERR_ALREADYREGISTRED(client->getNickname());
	if (cmd.size() < 3)
		err = ERR_NEEDMOREPARAMS(client->getNickname());
	else
	{
		client->setUsername(cmd[1]);
		client->setRealname(cmd[2]);
		return ;
	}
	this->sendMSG(err, client->getFd()); 
}

void  Server::privmsg(Client *client, std::vector<std::string> cmd)
{
	std::string		err;
	Client			*rcv;
	Channel			*chan;    
	std::string		channel;
	std::string		msg;

	if (!client->isRegistered())
		return (sendMSG(ERR_NOTREGISTERED(client->getNickname()), client->getFd()));
	else if (cmd.size() < 3)
		return (sendMSG(ERR_NEEDMOREPARAMS(client->getNickname()), client->getFd()));
  else if (cmd[1].at(0) == '#')
  {
    if (findChannel(cmd[1].substr(1)))
			return (sendToChannel(client->getPrivmsgParam(), client->getNickname(), findChannel(cmd[1].substr(1)), client));
		return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
  }
  else
  {
			if ((rcv = this->findClient(cmd[1])) != NULL)
				return (sendMSG(":" + client->getNickname() + " PRIVMSG " + cmd[1] + " " + client->getPrivmsgParam(), rcv->getFd()));
			else
				return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), cmd[1]), client->getFd()));
  }



	// else
	// {
	// 	if (cmd[1].at(0) == '#')
	// 	{
	// 		// channel = cmd[1];
	// 		channel = cmd[1].substr(1);
	// 		chan =    findChannel(channel);
	// 		if (chan == NULL)
	// 			return (sendMSG(ERR_NOSUCHCHANNEL(client->getNickname(), cmd[1]), client->getFd()));
	// 		sendToChannel(cmd[2].substr(1), client->getNickname(), chan, client);
	// 		return ;
	// 	}
	// 	else
	// 	{
	// 		if ((rcv = this->findClient(cmd[1])) != NULL)
	// 		{
	// 			msg = ":" + client->getNickname() + " PRIVMSG " + cmd[1] + " " + cmd[2];
	// 			sendMSG(msg, rcv->getFd());
	// 			return ;
	// 		}
	// 		else
	// 		{
	// 			std::cout << "command[1]: " << "*" << cmd[1] << "*" << std::endl;
	// 			return (sendMSG(ERR_NOSUCHNICK(client->getNickname(), cmd[1]), client->getFd()));
	// 		}
	// 	}
	// }
	// this->sendMSG(err, client->getFd());
}

void Server::modei(Client *client, std::vector<std::string> cmd)
{
	sendMSG(":localhost MODE " + client->getNickname() + " +i", client->getFd());
} 

void  Server::pong(Client *client, std::vector<std::string> cmd)
{
	sendMSG("PONG " + cmd[1], client->getFd());
}
