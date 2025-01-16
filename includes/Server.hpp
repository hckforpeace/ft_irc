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

#define MAX_EVENTS 1000

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

 	public:
		Server(char *port, char *password);
		void	init_server();
		int		setnonblocking(int sock);
		void	launch_server();
};

#endif