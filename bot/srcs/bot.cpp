#include <bot.hpp>

Bot::Bot(int port_number, std::string ipaddr, std::string nickname, std::string channel_name, std::vector<std::string> forbidden_words)
{
	this->port_number = port_number;
	this->ip_addr = ipaddr;
	this->nickname = nickname;
	this->channel_name = channel_name;
	this->forbidden_words = forbidden_words;
	connect_to_server();
}

void	Bot::connect_to_server()
{
	this->bot_socket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port_number);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

	connect(bot_socket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

	while (1)
	{

	}
}
