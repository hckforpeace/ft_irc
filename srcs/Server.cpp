#include "Server.hpp"

Server::~Server()
{
	std::cout << "destructeur sever " << std::endl;
	std::cout << "number of clients: " << Clients.size() << std::endl;
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		
		std::cout << "destroying" << std::endl;
		delete *it;
	}
}

Server::Server(char *port, char *password)
{
	parse_args(port, password);
	init_server();
	setupSignals();
	launch_server();
	// do stuff;
}

int Server::setnonblocking(int sock)
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



void	Server::launch_server()
{
	int nbr_fds;
	int con_sock;
	static std::string error;
	// epoll file descriptor
	int epfd;
	epfd = epoll_create1(0);
	if (epfd == -1)
	{
		error = "ERROR !! epoll_create1 => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}
	std::cout << "epfd: " << epfd << std::endl;

	// this is for the server
	ev.events = EPOLLIN;
	ev.data.fd = server_socket;

	// added the server_fd to the interest list watching out to READ from IT !
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &ev) == -1)
		std::cerr << "Error during epoll_ctl => " << strerror(errno) << std::endl;
	char buffer[512];
	bool loop = true;	
	int len;
	struct stat buf;
	while (!server_off)
	{
		nbr_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (errno == EAGAIN)
			std::cout << "tell me" << std::endl;
		for (int i = 0;  i < nbr_fds; ++i)
		{
			// std::cout << "Something happened event => " << events[i].events << std::endl;
			if (fstat(events[i].data.fd, &buf) == 0 && errno == EBADF)
				break;
			if (events[i].data.fd == server_socket)
			{
				std::cout << "events[i].data.fd: " << events[i].data.fd << std::endl;
				std::cout << "somebody is trying to connect !" << std::endl;
				socklen_t client_len = sizeof(client_addr);
				con_sock = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
				// write();
				if (con_sock == -1)
				{
					error = "ERROR !! WHILE CONNECTING TO NEW CLIENT SOCKET => ";
					error += strerror(errno);
					throw std::runtime_error(error);
				}
				ev.data.fd = con_sock;
				ev.events = EPOLLIN | EPOLLOUT;
				setnonblocking(con_sock);
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, con_sock, &ev) == -1)
				{
					error = "ERROR !! WHILE ADDING NEW EVENT TO EPOLL INTEREST LIST => ";
					error += strerror(errno);
					throw std::runtime_error(error);
				}
				Client *new_connection = new Client(con_sock);
				Clients.push_back(new_connection);
				std::cout << "success !! con_sock " << con_sock <<  " Connected" << std::endl;
			}
			else if (events[i].events == EPOLLOUT && loop)
			{
				std::cout << "Password: " << std::endl;
				send(con_sock, "Insert Password: ", sizeof(char) * 18, 0);
				loop = false;
			}
			else if (events[i].events == (EPOLLIN | EPOLLOUT))
			{
				len = recv(events[i].data.fd, buffer, 512 * sizeof(char), 0);
				if (len == 0)
				{
					std::cout << "An error has occured during recv of the client socket => " << events[i].data.fd << std::endl ;
					if (close(events[i].data.fd) == -1)
						std::cerr << "Failed to close socket" << std::endl;
					break ;
				}
				else
				{
					buffer[len] = '\0';
					std::cout << "message len: " << len << ", fd: " << events[i].data.fd << " event: " << events[i].events	 << " sent: " << buffer;
				}
				memset(buffer, 0, sizeof(char) * 512);
			}
		}
	}
	std::cout << "out of the loop" << std::endl;
	if (!close(server_socket))
		std::cout << "Successfully closed server_socket" << std::endl;
	if (!close(epfd))
		std::cout << "Successfully closed epfd" << std::endl;

}


/*
*/
void	Server::init_server()
{
	static std::string error;

	// With the flag SOCK_NONBLOCK he socket is nonblockant no need for fnctl
	this->server_socket = socket(AF_INET , SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->server_socket == -1)
	{
		error = "ERROR !! WHILE CREATING SOCKET => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}
	std::cout << GREEN << "Server Socket: " << BLU << server_socket << RESET << std::endl;

	// setting up server parameters
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// bind
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		close(server_socket);
		error = "ERROR !! WHILE BINDING => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}

	// listening
	if (listen(server_socket, 40) == -1)
	{	
		// TODO
		error = "ERROR !! WHILE LISTENING => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}
}

void	Server::parse_args(char *port, char *password)
{
	std::string 		s_password(password);
	std::stringstream	ss(port);
	int					port_nb;
	
	ss >> port_nb;
	if (ss.fail())
		throw std::runtime_error("Port is not a number");
	if (port_nb > 65535)
		throw std::runtime_error("Port must be between 0 and 65535");
	this->_port = port_nb;
	this->_password = s_password; //password has a length limit?
}


void 	Server::setupSignals()
{
	signal(SIGINT, signIntHandler);
}

void	Server::signIntHandler(int code)
{
	if (code == 2)
		server_off = true;
}