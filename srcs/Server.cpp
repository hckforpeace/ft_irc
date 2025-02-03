#include "Server.hpp"

// Constructor starts the server
Server::Server(char *port, char *password)
{
	parse_args(port, password);
	init_server_socket();
	init_server();
	setupSignals();
	run_server();
	// do stuff;
}

// Destructor Deletes Clients and Channels
Server::~Server()
{
	std::cout << "destructor sever" << std::endl;
	std::cout << "number of clients: " << Clients.size() << std::endl;
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		std::cout << "destroying" << std::endl;
		delete *it;
	}
	for (std::vector<Channel *>::iterator it = Channels.begin(); it != Channels.end(); it++)
		delete *it;
}

// Creates socket on which the server will listen for incoming connections
void	Server::init_server_socket()
{
	static std::string error;
	int				opt = 1;
	
	// With the flag SOCK_NONBLOCK he socket is nonblockant no need for fnctl
	this->server_socket = socket(AF_INET , SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->server_socket == -1)
	{
		error = "ERROR !! WHILE CREATING SOCKET => ";
		error += strerror(errno);
		throw std::runtime_error(error);
	}
	std::cout << GREEN << "Server Socket: " << BLU << server_socket << RESET << std::endl;

	// This option allows the server to bind to a socket address that is in the TIME_WAIT state.
    if (setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(this->server_socket);
        exit(EXIT_FAILURE);
    }

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

// Creates epoll instance
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

// stes an fd or a socket to nonblocking
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

// General Loop for server
void	Server::run_server()
{
	// looping 
	int nbr_fds;
	static std::string error;
	int len;
	struct stat buf;
	while (!server_off)
	{
		nbr_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		check_connection();
		if (errno == EAGAIN)
			std::cout << "tell me" << std::endl;
		for (int i = 0;  i < nbr_fds; ++i)
		{
			if (fstat(events[i].data.fd, &buf) == 0 && errno == EBADF)
				break;
			if (events[i].data.fd == server_socket)
			{
				std::cout << "Connection request" << std::endl;
				first_connection(nbr_fds, i);
			}
			
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

std::vector<Client *>::iterator	Server::getClientIt(int fd)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if ((*it)->getFd() == fd)
			return (it);
	}
	return (Clients.begin());
}

std::vector<Channel *>::iterator Server::getChannelIt(std::string name)
{
	for (std::vector<Channel *>::iterator it = Channels.begin(); it != Channels.end(); it++)
	{
		if (!name.compare((*it)->getName()))
			return (it);
	}
	return (Channels.begin());
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
	//send(con_socket, "Insert Password: ", sizeof(char) * 18, 0);
}

void	Server::read_and_process(int i)
{
	int	len;
	std::string str;
	Client *client = getClient(events[i].data.fd);
	int client_socket = client->getFd();

	// reading string sent from the Client
	len = recv(events[i].data.fd, buffer, 512 * sizeof(char), 0);
	std::cout << "DATA: " << buffer << std::endl;
	// Error
	if (len == 0)
	{
		std::cout << "An error has occured during recv of the client socket => " << events[i].data.fd << std::endl ;
		// this->Clients.erase(getClientIt(client->getFd()));
		std::cout << "Client deleted !" << std::endl;
		epoll_ctl(epfd, EPOLL_CTL_DEL, client->getFd(), &events[i]);
		destroy_cli_chan(client);

		return ;
	}
	else
	{
		buffer[len] = '\0';
	}

	str = buffer;
	if (isCRLF(str, client))
	{
		// Server outputs...
		std::cout << BLU "[CLIENT] " << client_socket << " => " RESET << YEL << client->getMessage() << RESET <<std::endl;


		const char *mess = (client->getMessage()).c_str();
		std::vector<std::string> lines = split_line_buffer(mess);
		int nbr_lines = lines.size();
		
		if (lines.size() > 1)
		{
			int i = 0;
			while (i < lines.size() && isOpenedSock(client_socket))
			{
				parse_exec_cmd(split_buffer(lines[i]), client);
				i++;
			}
		}
		else if (client->getMessage().compare(""))
		{
			if (!split_buffer(client->getMessage())[0].compare("PRIVMSG"))
				client->setPrivmsgParam(str); 
			parse_exec_cmd(split_buffer(client->getMessage()), client);
		}
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

std::vector<std::string> Server::split_line_buffer(const char *sentence)
{
  	std::stringstream ss(sentence);
  	std::vector<std::string> message;
	std::string	line;
	if (sentence != NULL)
	{
		while(std::getline(ss,line,'\n'))
			message.push_back(line);
	}
	return (message);
}

void		Server::processMessage(std::string str, Client *client)
{
	if (!client->isConnected())
	{
		if (!this->_password.compare(client->getMessage()))
		{
			std::cout << "Welcome" << std::endl;
			client->setConnection();
		}
		else
		{
			std::cout << RED << "Failed to Connect" << RESET << std::endl;
		}
	}
}

void	Server::parse_exec_cmd(std::vector<std::string> cmd, Client *client)
{
  	int fd = client->getFd();
	// else if(cmd.size() != 0 && (cmd[0] == "MODE" || cmd[0] == "MODE"))
	// 	modei(client, cmd); // welcome invisible user mode msg
	if (cmd.size() != 0 && (cmd[0] == "pass" || cmd[0] == "PASS"))		
		authenticate(client, cmd);// authenticate
	else if(cmd.size() != 0 && (cmd[0] == "WHOIS" || cmd[0] == "WHOIS"))
    whoIs(client, cmd); // welcome invisible user mode msg
	else if (cmd.size() != 0 && (cmd[0] == "ping" || cmd[0] == "PING"))
    	pong(client, cmd); // answer to ping
	else if (cmd.size() != 0 && (cmd[0] == "nick" || cmd[0] == "NICK"))
		setNickname(client, cmd); // set nickname
	else if (cmd.size() != 0 && (cmd[0] == "user" || cmd[0] == "USER"))
		setUser(client, cmd); // set username
	else if (cmd.size() != 0 && (cmd[0] == "join" || cmd[0] == "JOIN"))
		join(client, cmd); // join a channel
	else if (cmd.size() != 0 && (cmd[0] == "invite" || cmd[0] == "INVITE"))
		invite(client, cmd); // invite a client to a channel;
	else if (cmd.size() != 0 && (cmd[0] == "topic" || cmd[0] == "TOPIC"))
		topic(client, cmd); // set or change the topic
	else if (cmd.size() != 0 && (cmd[0] == "kick" || cmd[0] == "KICK"))
		kick(client, cmd); // kick a client from a channel
	else if (cmd.size() != 0 && (cmd[0] == "part" || cmd[0] == "PART"))
		part(client, cmd); // quiting a channel without quiting the server
	else if (cmd.size() != 0 && (cmd[0] == "mode" || cmd[0] == "MODE"))
		mode(cmd, client); // change the modes of a channel or client (user operator)
	else if (cmd.size() != 0 && (cmd[0] == "privmsg" || cmd[0] == "PRIVMSG"))
    	privmsg(client, cmd); // sends a msg to a client or to a channel
	else if (cmd.size() != 0 && (cmd[0] == "quit" || cmd[0] == "QUIT"))
    	quit(client, cmd);
	else if (cmd.size() != 0)
		sendMSG(ERR_UNKNOWNCOMMAND(client->getNickname(), cmd[0]), client->getFd());
	
	if (isOpenedSock(fd))
		client->setMessage("");

}
