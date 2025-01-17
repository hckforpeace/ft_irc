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
	public:
		Client(int fd);
		~Client();

};

#endif