#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <signal.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstddef>

#include "Colors.hpp"
#include "Client.hpp"
#include "Replies.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 1000

extern bool server_off;

class Server {
	private:
		int			_port;
		std::string	error;
		std::string	_password;
		char		buffer[512];
		int			epfd;
		int			server_socket;
 		int			con_socket;
		epoll_event	ev;
		epoll_event	events[MAX_EVENTS];
		sockaddr_in	server_addr;
 		sockaddr_in	client_addr;
		std::vector<Client*>	Clients;
		std::vector<Channel*>	Channels;

 	public:
		Server(char *port, char *password);
		~Server(void);

		// Server Methods
		void		init_server_socket();
		void		init_server();
		void		run_server();
		int			setnonblocking(int sock);
		void		first_connection();
		void		read_and_process(int i);

		// Signals
		void 		setupSignals();
		static void signalsHandler(int code);

    	// getters
		Client*								getClient(int fd);
		std::vector<Client *>::iterator		getClientIt(int fd);
		std::vector<Channel *>::iterator	getChannelIt(std::string name);

		// Parsers => parser.cpp
		void		parse_args(char *port, char *password);
    	void	parse_exec_cmd(std::vector<std::string> cmd, Client *client);
		std::vector<std::string> split_buffer(std::string str);
		std::vector<std::string> split_line_buffer(const char *sentence);
		
		// First Commands => First_Commands
		void	authenticate(Client *client, std::vector<std::string> cmd);
		void	setNickname(Client *client, std::vector<std::string> cmd);
		void	setUser(Client *client, std::vector<std::string> cmd);
		void 	pong(Client *client, std::vector<std::string> cmd);
		void 	whoIs(Client *client, std::vector<std::string> cmd);
		void 	quit(Client *client, std::vector<std::string> cmd);
		void 	updateNickInChan(Client *client, std::string oldNick, std::string newNick);
    

		// Modes
		void	mode(std::vector<std::string> cmd, Client *client);
		void	select_mode(char sign, char mode, std::vector<std::string> cmd, Client *client, Channel *channel);

		// Channel
		void	join(Client *client, std::vector<std::string> cmd);
		void	createChannel(std::string name, Client *client);
		void	enterChannel(Channel *channel, Client *client, std::string password);
	  	void	privmsg(Client *client, std::vector<std::string> cmd);
		bool 	isOperator(Client *client, Channel *channel);

		// CMD
		void		invite(Client *client, std::vector<std::string> cmd);
		void		topic(Client *client, std::vector<std::string> cmd);
		void		part(Client *client, std::vector<std::string> cmd);
		void		kick(Client *client, std::vector<std::string> cmd);

    	// utils
    	bool		nickInUse(std::string nickname);
		bool		isCRLF(std::string str, Client *client);
    	Client*		findClient(std::string nickname);
		Channel*	findChannel(std::string channel_name);
		void   		sendMSG(std::string message, int fd);
		void  		sendToChannel(std::string message, Channel *channel, Client *client);
		void    	sendMSGChan(std::string message, Channel *channel);
		void    	sendChanExptcli(Channel *chan, std::string msg, Client *client);		
		bool		isinChan(Client *client, Channel *channel);
		void    	send_to_all_client(std::string message);
		void		removeClient(Client *client);
		void		removeChan(Channel *channel);
    	bool    	isOpenedSock(int socket);
    	void    	destroy_cli_chan(Client *client);
    	std::string genWhoisRpl(std::string client, std::string nick);

		// mode
		void		inviteMode(char sign, Client *client, Channel *channel);
		void		topicMode(char sign, Client *client, Channel *channel);
		void		keyMode(char sign, std::string password, Client *client, Channel *channel);
		void		operatorMode(char sign, std::string new_operator, Client *client, Channel *channel);
		void		limitMode(char sign, std::string limit, Client *client, Channel *channel);
		std::string	generateNick(std::string base);
  		void		check_connection();
};

#endif
