#include "Replies.hpp"
#include <Server.hpp>
#include <string>

// Commands
void	Server::authenticate(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	if (cmd.size() < 2)
		err = ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]);
	else if (client->isConnected())
		err = ERR_ALREADYREGISTRED(client->getNickname());
	else
	{
		if (!cmd[1].compare(this->_password))
		{
			client->setPasswordInserted();
			return ;
		}
		else
			err = ERR_PASSWDMISMATCH(client->getNickname());
	}
	this->sendMSG(err, client->getFd()); 
}

void	Server::setNickname(Client *client, std::vector<std::string> cmd)
{
	std::string		err;
	std::string 	old_nick;

	// if (!client->isConnected())
	// 	err = ERR_NOTREGISTERED(client->getNickname());
	if (cmd.size() < 2)
		err = ERR_NONICKNAMEGIVEN(client->getNickname());
	else
	{
		if (client->getNickname().compare(""))
			old_nick = client->getNickname();
		else
			old_nick = cmd[1];

		if (nickInUse(cmd[1]))
		{
			old_nick = cmd[1];
			cmd[1] = generateNick(cmd[1]);
		}


		std::string temp = cmd[1];
		if (temp.at(0) == ':' || temp.at(0) == '#')
			err = ERR_ERRONEUSNICKNAME(cmd[1]);
		else
    	{
			client->setNickname(cmd[1]);
			std::cout << "Setting A new nick to fd: " << client->getFd() << ", to: " << cmd[1] << std::endl;
			std::cout << RED "MESSAGE SENT: "  RESET << " :" + old_nick + "!" + client->getUsername() + "@localhost " + " NICK " + cmd[1] << std::endl; 
 			// sendMSG(": " + old_nick + " NICK " + cmd[1], client->getFd());
 			sendMSG(":" + old_nick + "!" + client->getUsername() + "@localhost " + " NICK " + cmd[1], client->getFd());
			return ;
		}
	}
  	std::cout << "SetNickname error message sends: " << err << std::endl;
	this->sendMSG(err, client->getFd()); 
}

void	Server::setUser(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	// if (!client->isConnected())
	// 	err = ERR_NOTREGISTERED(client->getNickname());
	if (cmd.size() < 3)
		err = ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]);
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
	Client        *rcv;
	Channel       *chan;    
	std::string   channel;
	std::string   msg;

	if (!client->isRegistered())
		err = ERR_NOTREGISTERED(client->getNickname());
	else if (cmd.size() < 3)
		err = ERR_NEEDMOREPARAMS(client->getNickname(), cmd[0]);
	else
	{
		if (cmd[1].at(0) == '#')
	{
		channel = cmd[1];
		channel = channel.substr(1);
		chan =    findChannel(channel);
		sendToChannel(cmd[2], client->getNickname(), chan, client);
		return ;
	}
		else
		{
			if ((rcv = this->findClient(cmd[1])) != NULL)
		{
			msg = "<" RED + client->getNickname() +  RESET "r" + " " + cmd[2];
			sendMSG(msg, rcv->getFd());
			return ;
		}
			else
				err = ERR_NOSUCHNICK(cmd[1]);
		
	}
	}
	this->sendMSG(err, client->getFd());
}


void	Server::send_to_all_client(std::string message)
{
	std::cout << "To all clients" << message << std::endl;
	for (std::vector<Client*>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		std::cout << "sending to client with fd: " << (*it)->getFd() << std::endl;
		sendMSG(message, (*it)->getFd());
	}
}

void Server::modei(Client *client, std::vector<std::string> cmd)
{
	sendMSG(":localhost MODE " + client->getNickname() + " +i", client->getFd());
} 