#ifndef REPLIES_HPP
# define REPLIES_HPP

// Command responses

/*================ PREFIX ====================*/
# define PREFIX ":localhost "

/*================ CHANNEL ====================*/

// Welcome message when a client joins a channel
# define CHAN_WELC(nickname, channel) "<" + nickname + "> joined <"  + channel + "> channel" 

/*================= ERRORS ===================*/

# define ERR_NONICKNAMEGIVEN(nick) PREFIX "431 " + nick + " :No nickname given"

//Returned to a registered client to indicate that the command sent is unknown by the server.
# define ERR_UNKNOWNCOMMAND(command) "421 " + command + " :Unknown command" 

// Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOTREGISTERED(nickname) "451 " + nickname + " : You have not registered" 

// Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
# define ERR_NEEDMOREPARAMS(user, command) PREFIX  "461 " + user + " " + command + " :Not enough parameters" 

// Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
# define ERR_ALREADYREGISTRED(user) PREFIX "462 " + user + " :You may not reregister" 


# define ERR_PASSWORDNOTINSERTED "42 :You didn't insert the password\n"

// Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
# define ERR_PASSWDMISMATCH(nick) PREFIX "464 " + nick + " :Password incorrect" 

// Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_NOPRIVILEGES "481 :Permission Denied- You're not an IRC operator" 

// Indicates that no client can be found for the supplied nickname.
# define ERR_NOSUCHNICK(nickname) "401 " + nickname + " :No such nickname" 

# define ERR_NICKNAMEINUSE(nickname) ": 433 " + nickname + " :Nickname is already in use" 

# define ERR_ERRONEUSNICKNAME(nickname) "432 " + nickname + " :Erroneus nickname"  

// Indicates that no channel can be found for the supplied channel name.
# define ERR_NOSUCHCHANNEL(channel) "403 " + channel + " :No such channel" 

// Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send.
# define ERR_NOTEXTTOSEND "412 <client> :No text to send" 

// Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).
# define ERR_INPUTTOOLONG "417 <client> :Input line was too long" 

//Channel errors

// Returned when a client tries to invite a user to a channel they are already on.
# define ERR_USERONCHANNEL(user, channel) "443 " + user + channel + " :is already on channel" 

// Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_TOOMANYCHANNELS(channel) "405 " + channel + " :You have joined too many channels" 

# define ERR_CHANNELISFULL(channel) "471 " + channel + " :Cannot join channel (+l)" 

# define ERR_UNKNOWNMODE(char) "472 " + char + " :is unknown mode char to me" 

# define ERR_INVITEONLYCHAN(channel) "473 " + channel + " :Cannot join channel (+i)" 

// Returned when the "join #channel" syntax is not respected
# define ERR_INVCHANNELNAME "The channel name must be preceded by an '#' to be interpreted" 

#endif
