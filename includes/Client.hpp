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
		// std::string	denied_nick;
		std::string message;
		std::vector<std::string>	cmd;
		int			fd;
		int			channel_counter; // max 10
		bool		connected; // false by default
		bool		password_inserted; // false by default

	public:
		Client(int fd);
		~Client();
		int	getFd();
		std::vector<std::string> getCmd() {return (this->cmd);};
		std::string&	getMessage();

		void	setMessage(std::string str);
		void	setConnection();
		void	setCmd(std::vector<std::string> cmd) {this->cmd = cmd;};
		void	setNickname(std::string nick);
		void	setPasswordInserted();

		std::string	getNickname();
		std::string	getUsername();  
		std::string	getRealname();
		std::string getDieniedNick();
		bool		getPassstatus();
		bool	isConnected();
  		bool 	isRegistered();


		void		setUsername(std::string str);
		void		joinChanCounter(void);
		void		setRealname(std::string str);
};

#endif
