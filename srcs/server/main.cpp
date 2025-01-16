#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  // For inet_ntop
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstddef>
#include <fcntl.h>

#define MAX_EVENTS 1

int setnonblocking(int sock)
{
    int result;
    int flags;

    flags = ::fcntl(sock, F_GETFL, 0);

    if (flags == -1)
		return -1; // error

    flags |= O_NONBLOCK;

    result = fcntl(sock , F_SETFL , flags);
    return result;
}

int main() 
{
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];

	int nbr_fds, con_sock;

	// With the flag SOCK_NONBLOCK he socket is nonblockant no need for fnctl
	int server_socket = socket(AF_INET , SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (server_socket == -1)
		std::cerr << "Error during socket creation !" << std::endl;
	std::cout << "server Socket: " << server_socket << std::endl;


	// server address
	sockaddr_in server_addr;
	// client address
	sockaddr_in client_addr;

	// setting up server parameters
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4040);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// bind
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cerr << "Error during binding =>" << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	// listening
	if (listen(server_socket, 40))
	{
		std::cerr << "listen " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// epoll file descriptor
	int epfd;
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
	char buffer[1000];
	bool loop = true;
	int len;
	while (1)
	{
		nbr_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		for (int i = 0;  i < nbr_fds; ++i)
		{
			// std::cout << "Something happened event => " << events[i].events << std::endl;
			// if (events[i].events == EPOLLHUP)
				// std::cout << events[i].data.fd << "was disconnected !" << std::endl;
			if (events[i].data.fd == server_socket)
			{
				std::cout << "events[i].data.fd: " << events[i].data.fd << std::endl;
				std::cout << "somebody is trying to connect !" << std::endl;
				socklen_t client_len = sizeof(client_addr);
				con_sock = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
				// write();
				if (con_sock == -1)
				{
					std::cout << "con_sock: " << strerror(errno) << "con_sock: " << con_sock << std::endl;
					exit(EXIT_FAILURE);
				}
				std::cout << "success !! con_sock " << con_sock << std::endl;
				events[i].data.fd = con_sock;
				events[i].events = EPOLLIN | EPOLLET | EPOLLOUT;
				setnonblocking(con_sock);
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, con_sock, &events[i]) == -1)
				{
					std::cout << strerror(errno) << std::endl;
					exit(EXIT_FAILURE);
				}
				// send(con_sock, "Insert Password: ", sizeof(char) * 18, 0);
			}
			else if (events[i].events == EPOLLOUT && loop)
			{
				std::cout << "Password: " << std::endl;
				loop = false;
			}
			else if (events[i].events == (EPOLLIN | EPOLLOUT))
			{
				len = read(events[i].data.fd, buffer, 1000);
				buffer[len] = '\0';
				std::cout << "fd: " << events[i].data.fd << " event: " << events[i].events	 << " sent: " << buffer;
				// std::cerr << "start" << std::endl;
				memset(buffer, 0, sizeof(char) * 1000);
				// std::cerr << "end" << std::endl;
			}
		}
	}

	return (0);
}

