#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdbool.h>

class Client 
{
	private:
		std::string nickname;
		std::string username;
		int			fd;
		bool		connected;

	public:
		Client(void) {};
};

#endif