#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>
#include <vector>

class Client 
{
	private:
		std::string realname;
		std::string username;
		std::string nickname;
		std::string message;
		std::vector<std::string>	cmd;
		int			fd;
		int			channel_counter; // max 10
		bool		connected; // false by default

	public:
		Client(int fd);
		~Client();
		int	getFd();
		std::vector<std::string> getCmd() {return (this->cmd);};
		std::string&	getMessage();
		bool	isConnected();
    bool  isRegistered();
		void	setMessage(std::string str);
		void	setConnection();
		void	setCmd(std::vector<std::string> cmd) {this->cmd = cmd;};
		void	setNickname(std::string nick);
		std::string	getNickname();
		std::string	getUsername();  
		std::string	getRealname();

		void		setUsername(std::string str);
		void		joinChanCounter(void);
		void		setRealname(std::string str);
};

#endif
