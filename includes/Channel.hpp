#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Colors.hpp"
#include "Client.hpp"
#include "Replies.hpp"

class Channel
{
	private:
		int			limit; // how many clients the channel can support
		std::string name;
		std::string topic_name;
		std::string password; // if there's a key
		std::vector<Client> Clients;
		std::vector<Client> operators;
	public:
		Channel();
		~Channel();
		Channel(Channel const &src);
		Channel &operator=(Channel const &src);

		/*Getters*/
		std::string	getName(void);

		/*Setter*/
		void	setName(std::string name);

		/*methods*/
		void	add_client(Client new_client);
		void	add_operator(Client new_operator);
};

#endif
