#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

class Bot
{
  private:
	int			port_number;
	std::string	ip_addr;
    std::string nickname;
    std::string channel_name;
    std::vector<std::string> forbidden_words;
	int						bot_socket;
	sockaddr_in			serverAddr;
  public:
    Bot(int port_number, std::string ipaddr, std::string nickname, std::string channel_name, std::vector<std::string> forbidden_words);
	void	connect_to_server();
};

#endif // DEBUG
