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
		std::vector<std::string>	invited_to_chan;
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
		bool		isConnected(void);
    	bool		isRegistered(void);
		std::string	getNickname(void);
		std::string	getUsername(void);  
		std::string	getRealname(void);
		std::string getHostname(void);
		std::string getDeniedNick(void);
		int			getChanCounter(void);
		bool		getPassstatus(void);

		void	setMessage(std::string str);
		void	setConnection(void);
		void	setCmd(std::vector<std::string> cmd) {this->cmd = cmd;};
		void	setNickname(std::string nick);
		void	setUsername(std::string str);
		void	setChanCounter(void);
		void	setRealname(std::string str);
		void	addtoInviteChan(std::string channel);
		bool	isInvited(std::string channel);
		void	setPasswordInserted(void);
		void	joinChanCounter(void);
};

#endif
