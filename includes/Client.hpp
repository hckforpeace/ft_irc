#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>


class Client 
{
	private:
		std::string name;
		std::string nickname;
		std::string message;
		int			fd;
		bool		connected = false;
	public:
		Client(int fd);
		~Client();
		int	getFd();
		std::string&	getMessage();
		bool	isConnected();
		void	setMessage(std::string str);
		void	setConnection();
};

#endif