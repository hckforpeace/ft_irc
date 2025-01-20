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
	init_server_socket();
	init_server();
	setupSignals();
	run_sever();
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



void	Server::init_server()
{
	static std::string error;
	// epoll file descriptor
	epfd = epoll_create1(0);
	if (epfd == -1)
	{
		error = "ERROR !! epoll_create1 => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}
	std::cout << GREEN << "epfd: " << RESET << epfd << std::endl;

	// this is for the 
	ev.events = EPOLLIN;
	ev.data.fd = server_socket;

	// added the server_fd to the interest list watching out to READ from IT !
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &ev) == -1)
		std::cerr << "Error during epoll_ctl => " << strerror(errno) << std::endl;
}

void	Server::run_sever()
{
	// looping 
	int nbr_fds;
	static std::string error;
	int len;
	struct stat buf;
	while (!server_off)
	{
		nbr_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		// if (errno == EAGAIN)
			// std::cout << "tell me" << std::endl;
		for (int i = 0;  i < nbr_fds; ++i)
		{
			if (fstat(events[i].data.fd, &buf) == 0 && errno == EBADF)
				break;
			if (events[i].data.fd == server_socket)
			{
				std::cout << "Connection request" << std::endl;
				first_connection(nbr_fds, i);
			}
			// else if (events[i].events == EPOLLOUT && getClient(events[i].data.fd)->isConnected())
			// {
			// 	loop = false;
			// }
			
			// socket available for read operation 
			else if (events[i].events == (EPOLLIN | EPOLLOUT))
				read_and_process(i);
		}
	}
	std::cout << "out of the loop" << std::endl;
	if (close(server_socket) == -1)
		std::cout << RED << "failed to closed server_socket" << RESET << std::endl;
	if (close(epfd) == -1)
		std::cout << RED << "Successfully closed epfd" << RESET << std::endl;
}

/*
*/
void	Server::init_server_socket()
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

Client*	Server::getClient(int fd)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);
}


void		Server::first_connection(int nbr_fds, int i)
{
	// std::cout << "events[i].data.fd: " << events[i].data.fd << std::endl;
	// std::cout << "somebody is trying to connect !" << std::endl;


	socklen_t client_len = sizeof(client_addr);
	con_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

	// error
	if (con_socket == -1)
	{
		error = "ERROR !! WHILE CONNECTING TO NEW CLIENT SOCKET => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}

	// setting up params the new event 
	ev.data.fd = con_socket;
	ev.events = EPOLLIN | EPOLLOUT;
	setnonblocking(con_socket);

	// adding the event to the interest list
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, con_socket, &ev) == -1)
	{
		error = "ERROR !! WHILE ADDING NEW EVENT TO EPOLL INTEREST LIST => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}

	// Creating and adding a new obj Client to the vector
	Client *new_connection = new Client(con_socket);
	Clients.push_back(new_connection);

	// std::cout << "success !! con_socket " << con_socket <<  " Connected" << std::endl;
	send(con_socket, "Insert Password: ", sizeof(char) * 18, 0);
}

void	Server::read_and_process(int i)
{
	int	len;
	std::string str;

	// reading string sent from the Client
	len = recv(events[i].data.fd, buffer, 512 * sizeof(char), 0);
	// Error
	if (len == 0)
	{
		std::cout << "An error has occured during recv of the client socket => " << events[i].data.fd << std::endl ;
		if (close(events[i].data.fd) == -1)
			std::cerr << "Failed to close socket" << std::endl;
	}
	else
	{
		buffer[len] = '\0';
	}

	str = buffer;
	Client *client = getClient(events[i].data.fd);
	if (isCRLF(str, client))
	{
		parse_exec_cmd(split_buffer(client->getMessage()), client);
		client->setMessage("");
	}
	memset(buffer, 0, sizeof(char) * 512);
}

std::vector<std::string> Server::split_buffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream	ss(str);
	std::string token;

	while (ss >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return (vec);
}

bool	Server::isCRLF(std::string str, Client *client)
{
	std::size_t pos = str.find("\r\n");
	std::string sub;
	
	if (pos == std::string::npos)
	{
		client->setMessage(client->getMessage() + str);
		return (false);
	}
	sub = str.substr(0, str.length() - 2);
	client->setMessage(client->getMessage() + sub);
	return (true);
}

void	Server::parse_exec_cmd(std::vector<std::string> cmd, Client *client)
{
	//std::cout << "cmd[0] -> " << cmd[0] << std::endl;
	if (cmd.size() != 0 && (cmd[0] == "pass" || cmd[0] == "PASS"))		
		std::cout << "test" << std::endl;// authenticate
	else if (cmd.size() != 0 && (cmd[0] == "nick" || cmd[0] == "NICK"))
		std::cout << "test" << std::endl;// set nickename
	else if (cmd.size() != 0 && (cmd[0] == "user" || cmd[0] == "USER"))
		std::cout << "test" << std::endl;// set username
	else if (cmd.size() != 0 && (cmd[0] == "join" || cmd[0] == "JOIN"))
		std::cout << "test" << std::endl;// join channel
	else if (cmd.size() != 0 && (cmd[0] == "invite" || cmd[0] == "INVITE"))
		std::cout << "test" << std::endl;// invite client to channel
	else if (cmd.size() != 0 && (cmd[0] == "topic" || cmd[0] == "TOPIC"))
		std::cout << "test" << std::endl;// change the topic of a channel
	else if (cmd.size() != 0 && (cmd[0] == "kick" || cmd[0] == "KICK"))
		std::cout << "test" << std::endl;// kick a client from a channel
	else if (cmd.size() != 0 && (cmd[0] == "mode" || cmd[0] == "MODE"))
		std::cout << "test" << std::endl;// change the modes of a channel or client (user operator)
	else if (cmd.size() != 0 && (cmd[0] == "privmsg" || cmd[0] == "PRIVMSG"))
		std::cout << "test" << std::endl;//send a mp or a msg to a channel
	else if (cmd.size() != 0 && (cmd[0] == "quit" || cmd[0] == "QUIT"))
		std::cout << "test" << std::endl;// quit the server
	else if (cmd.size() != 0)
	{
		std::string err = ERR_UNKNOWNCOMMAND(cmd[0]);
		send(con_socket, err.c_str(), sizeof(err), 0);
	}
}

// if (!client->isConnected())
// {
// 	if (!this->_password.compare(client->getMessage()))
// 	{
// 		std::cout << "Welcome" << std::endl;
// 		client->setConnection();
// 	}
// 	else
// 	{
// 		std::cout << "Failed to Connect" << std::endl;
// 		// TODO
// 		close(client->getFd());
// 		// this->Clients.erase(this->Clients.);
// 		// delete client;
// 	}
// }