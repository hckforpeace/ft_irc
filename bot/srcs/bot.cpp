#include <bot.hpp>

bool bot_off = false;

Bot::Bot(int port_number, std::string pass, std::string nick, std::string channel_name, std::vector<std::string> forbidden_words)
{
	this->port_number = port_number;
	this->pass = pass;
	this->nickname = nick;
	this->channel_name = channel_name;
	this->forbidden_words = forbidden_words;
	this->connected = false;
	connect_to_server();
}

void	Bot::connect_to_server()
{
	std::string server_msg;
	char buffer[512];
	memset(buffer, 0, 512);
	this->bot_socket = socket(AF_INET, SOCK_STREAM, 0);
	int	len;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port_number);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (connect(bot_socket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1)
	{
		close(bot_socket);
		server_msg = strerror(errno);
		throw std::runtime_error("ERROR COULD NOT REACH SERVER " + server_msg);
	}
	setnonblocking(bot_socket);

	std::string first_connection = "CAP LS\nPASS " + this->pass + "\nNICK pipoloni\nUSER "+ nickname + " " + nickname;
	sendMSG(first_connection, bot_socket);
	signal(SIGINT, handler);
	// len = recv(bot_socket, buffer, 512 * sizeof(char), 0);
	// if (len == -1)
	// 	bot_off = false;
	// std::string server_msg = buffer;
	// if (!split_buffer(server_msg)[1].compare(""))

	while (!bot_off)
	{
		if (!isOpenedSock(bot_socket))
			break;
		len = recv(bot_socket, buffer, 512 * sizeof(char), 0);
		if (len == 0)
			bot_off = true;
		server_msg = buffer;
		try
		{
			process_messages(server_msg);
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;;
			break ;
		}
		
		memset(buffer, 0, 512);
	}
	close(bot_socket);
}

void Bot::sendMSG(std::string message, int fd)
{
	const char *buffer;
	message.append("\r\n");

	if (!isOpenedSock(fd))
	{
		bot_off = true;
		return ;
	}
	std::cout << RED "[Bot-To-Server] <" << fd << "> => " RESET << GREEN << message << RESET << std::endl;
	buffer = message.c_str();
	if (send(fd, buffer, message.length(), 0) == -1)
		std::cout << RED << "send() failed" << RESET << std::endl;
}

bool    Bot::isOpenedSock(int socket)
{
  struct stat statbuf;

  // Call fstat on the socket file descriptor
  if (fstat(socket, &statbuf) == -1) 
    return (false); // If fstat fails, the socket is likely closed or invalid
  return (true);
}

void  Bot::process_messages(std::string message)
{
	std::vector<std::string> arr = split_buffer(message);
	if (arr.size() > 2 && arr[1].compare("464"))
		std::runtime_error("ERROR: Password Incorret");
	else if (arr.size() >= 2 && arr[1].compare("001"))
	{
		sendMSG("JOIN #" + this->channel_name, this->bot_socket);
		this->connected = true;
	}
	// else if (arr.size() >= 3 && !arr[0].compare("PRIVMSG"))
}


std::vector<std::string> Bot::split_buffer(std::string str)
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

void    handler(int code)
{
	(void)code;
	bot_off = true;
}

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