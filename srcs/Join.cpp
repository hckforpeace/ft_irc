#include "Server.hpp"

void Server::join(std::vector<std::string> cmd, Client *client)
{
	int flag = 0;
	const char *buffer;
	if (cmd.size() < 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(cmd[0]);
		send(con_socket, err.c_str(), sizeof(err), 0);
		return;
	}
	if (cmd[1].empty() || cmd[1].at(0) != '#' || (cmd[1].at(0) == '#' && cmd.size() == 1))
	{
		std::string err = ERR_INVCHANNELNAME;
		buffer = err.c_str();
		send(con_socket, err.c_str(), sizeof(err), 0);
		return;
	}
	for (int i = 0; !this->Channels.empty() && this->Channels[i].getName() == cmd[0]; i++)
	{
		enterChannel(cmd[1].substr(1), client);
	}
	createChannel(cmd[1].substr(1), client);
}

void Server::createChannel(std::string name, Client *client)
{
	Channel	new_channel;

	new_channel.setName(name);
	new_channel.add_operator(*client);
	this->Channels.push_back(new_channel);
	sendMSG(CHAN_WELC(client->getNick(), name), client->getFd());
}

void Server::enterChannel(std::string name, Client *client)
{
	(void)name;
	(void)client;
}
