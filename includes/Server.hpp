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
#include "Colors.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <vector>
#include "Client.hpp"

#define MAX_EVENTS 1000

static bool server_off = false;

class Server {
	private:
		void	parse_args(char *port, char *password);
		int	_port;
		std::string	_password;
		int			epfd;
		int			server_socket;
 		int			con_socket;
		epoll_event	ev;
		epoll_event	events[MAX_EVENTS];
		sockaddr_in  server_addr;
 		sockaddr_in  client_addr;
		std::vector<Client*> Clients;

 	public:
		Server(char *port, char *password);
		~Server();
		void	init_server();
		int		setnonblocking(int sock);
		void 	setupSignals();
		void	launch_server();
		static void signIntHandler(int code);	
};

#endif