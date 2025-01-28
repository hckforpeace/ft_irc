#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <vector>

#include "Colors.hpp"
#include "Client.hpp"
#include "Replies.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 1000

static bool server_off = false;

class Server {
	private:
		int	_port;
		std::string	error;
		std::string	_password;
		char		buffer[512];
		int			epfd;
		int			server_socket;
 		int			con_socket;
		epoll_event	ev;
		epoll_event	events[MAX_EVENTS];
		sockaddr_in  server_addr;
 		sockaddr_in  client_addr;
		std::vector<Client*> Clients;
		std::vector<Channel*> Channels;
		void	parse_args(char *port, char *password);

 	public:
		Server(char *port, char *password);
		~Server();
		void	init_server_socket();
		void	init_server();
		void	run_server();
		int		setnonblocking(int sock);
		void 	setupSignals();
		static void signIntHandler(int code);
		void		first_connection(int nbr_fds, int i);
		void		read_and_process(int i);
		void		processMessage(std::string str, Client *client);

    	// getters
		Client*	getClient(int fd);
		std::vector<Client *>::iterator	getClientIt(int fd);

    	void		parse_exec_cmd(std::vector<std::string> cmd, Client *client);
		std::vector<std::string> split_buffer(std::string str);
		std::vector<std::string> split_line_buffer(const char *sentence);
		

		// commands execution
		void	authenticate(Client *client, std::vector<std::string> cmd);
		void	setNickname(Client *client, std::vector<std::string> cmd);
		void	setUsername(Client *client, std::vector<std::string> cmd);
		void	modei(Client *client, std::vector<std::string> cmd);

		// Channel
		void	join(std::vector<std::string>, Client *client);
		void	createChannel(std::string name, Client *client);
		void	enterChannel(Channel *channel, Client *client);
		void	setUser(Client *client, std::vector<std::string> cmd);
	  	void	privmsg(Client *client, std::vector<std::string> cmd);

    	// utils
    	bool	nickInUse(std::string nickname);
		bool	isCRLF(std::string str, Client *client);
		bool 	 isRegistered(Client *client);
    	Client*  findClient(std::string nickname);
		void     sendMSG(std::string message, int fd);
    	Channel*  findChannel(std::string channelname);
    	void    sendToChannel(std::string message, std::string nickname, Channel *chan, Client *client);
		bool	login_parse(std::vector<std::string> cmds, Client *client);
		void	send_to_all_client(std::string message);
		std::string	generateNick(std::string base);
		void	check_connection();
};

#endif
