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
		std::vector<std::string>	cmd;
		int			fd;
		bool		connected = false;
	public:
		Client(int fd);
		~Client();
		int	getFd();
		std::vector<std::string> getCmd() {return (this->cmd);};
		std::string&	getMessage();
		bool	isConnected();
		void	setMessage(std::string str);
		void	setConnection();
		void	setCmd(std::vector<std::string> cmd) {this->cmd = cmd;};
};

#endif