#ifndef BOT_HPP
#define BOT_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Colors.hpp"

extern bool bot_off;

class Bot
{
	private:
		int port_number;
		std::string pass;
		std::string nickname;
		std::string channel_name;
		std::vector<std::string> forbidden_words;
		int bot_socket;
		sockaddr_in serverAddr;
		bool authenticated;

	public:
		Bot(int port_number, std::string pass, std::string channel_name, std::vector<std::string> forbidden_words);
		void sendMSG(std::string message, int fd);
		void connect_to_server();
		bool isOpenedSock(int socket);
		void process_messages(std::string message);
		std::vector<std::string> split_buffer(std::string str);
		void createChan();
		bool isExplicit();
		int isForbidden(std::vector<std::string> vec);
		int isForbiddenWord(std::string vec);
};

void handler(int code);
int setnonblocking(int sock);

#endif
