#include "Replies.hpp"
#include <Server.hpp>
#include <string>

// Commands
void	Server::authenticate(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	if (cmd.size() < 2)
		err = ERR_NEEDMOREPARAMS(cmd[0]);
	else if (client->isConnected())
		err = ERR_ALREADYREGISTRED(cmd[0]);
	else
	{
		if (!cmd[1].compare(this->_password))
		{
			client->setConnection();
			return ;
		}
		else
			err = ERR_PASSWDMISMATCH;
	}
	this->sendMSG(err, client->getFd()); 
}

void	Server::setNickname(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	if (!client->isConnected())
		err = ERR_NOTREGISTERED(client->getNickname());
	else if (cmd.size() < 2)
		err = ERR_NEEDMOREPARAMS(cmd[0]);
	else if (nickInUse(cmd[1]))
		err = ERR_NICKNAMEINUSE(cmd[1]);
	else
	{
		std::string temp = cmd[1];
		if (temp.at(0) == ':' || temp.at(0) == '#')
			err = ERR_ERRONEUSNICKNAME(cmd[1]);
		else
    {
			client->setNickname(cmd[1]);
			return ;
		}
	}
	this->sendMSG(err, client->getFd()); 
}

void	Server::setUser(Client *client, std::vector<std::string> cmd)
{
	std::string		err;

	if (!client->isConnected())
		err = ERR_NOTREGISTERED(client->getNickname());
	else if (cmd.size() < 3)
		err = ERR_NEEDMOREPARAMS(cmd[0]);
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
		err = ERR_NEEDMOREPARAMS(cmd[0]);
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
        msg = "<" RED + client->getNickname() +  RESET ">" + " " + cmd[2];
        sendMSG(msg, rcv->getFd());
        return ;
      }
			else
				err = ERR_NOSUCHNICK(cmd[1]);
		
    }
	}
	this->sendMSG(err, client->getFd());
}
