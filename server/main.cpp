#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  // For inet_ntop
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10

int main() 
{
	struct epoll_event ev, events[MAX_EVENTS];
	int nbr_fds;
	// struct epoll_event ;

	// With the flag SOCK_NONBLOCK he socket is nonblockant no need for fnctl
	int server_socket = socket(AF_INET , SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (server_socket == -1)
		std::cerr << "Error during socket creation !" << std::endl;
	std::cout << "server Socket: " << server_socket << std::endl;

	sockaddr_in server_addr;
	// AF_INET is Ipv4 I TODO
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4040);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// bind
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cerr << "Error during binding" << std::endl;
		exit(EXIT_FAILURE);
	}
	// listening
	listen(server_socket, 40);

	int epfd;
	// sockaddr_in clientfd;

	// create instance of epoll
	epfd = epoll_create1(0);
	if (epfd == -1)
	{
		std::cerr << "Error during epoll_create" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "epfd: " << epfd << std::endl;

	// this is for the server
	ev.events = EPOLLIN;
	ev.data.fd = server_socket;

	// added the server_fd to the interest list watching out to READ from IT !
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &ev) == -1)
		std::cerr << "Error during epoll_ctl => " << strerror(errno) << std::endl;

	while (1)
	{
		nbr_fds = epoll_wait(epfd, events, MAX_EVENTS, 0);
		for (int i = 0;  i < nbr_fds; ++i)
		{
			std::cout << "i: " << i << std::endl;		
			if (events[i].data.fd == server_socket)
			{
				std::cout << "events[i].data.fd: " << events[i].data.fd << std::endl;
				std::cout << "somebody is trying to connect !";
			}
		}
	}

	return (0);
}

