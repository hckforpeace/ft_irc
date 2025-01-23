#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Colors.hpp"
#include "Client.hpp"
#include "Replies.hpp"

class Channel
{
	private:
		int			limit; // 512 by default but can be changed with mode +l, it has to be at least 1
		std::string name;
		std::string topic_name;
		std::string password; // if there's a key
		std::vector<Client *> chan_clients;
		std::vector<Client *> operators;
		bool		invite_only;
		bool		topic_mode; //neccesary? could use empty string as a flag
		bool		keypass_mode;
		bool		limit_mode;

	public:
		Channel(std::string name);
		~Channel();
		Channel(Channel const &src);
		Channel &operator=(Channel const &src);

		/*Getters*/
		std::vector<Client *> getClients(void);
		std::vector<Client *> getOperators(void);
		std::string	getName(void);
		int			getLimit(void);
		int			getTotalClient(void);

		/*Setter*/
		void	setName(std::string name);
		void	setLimit(int limit);

		/*methods*/
		void	add_client(Client *new_client);
		void	add_operator(Client *new_operator);
};

#endif
