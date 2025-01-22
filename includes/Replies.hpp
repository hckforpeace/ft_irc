#ifndef REPLIES_HPP
# define REPLIES_HPP

// Command responses

/*================ CHANNEL ====================*/

// Welcome message when a client joins a channel
# define CHAN_WELC(nickname, channel) GREEN "<" + nickname + "> joined <"  + channel + "> channel" RESET

/*================= ERRORS ===================*/

//Returned to a registered client to indicate that the command sent is unknown by the server.
# define ERR_UNKNOWNCOMMAND(command) RED "421 " + command + " :Unknown command" RESET

// Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOTREGISTERED(nickanme) RED "451 " + nickname + " : You have not registered" RESET

// Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
# define ERR_NEEDMOREPARAMS(command) RED "461 " + command + " :Not enough parameters" RESET

// Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
# define ERR_ALREADYREGISTRED(user) RED "462 " + user + " :You may not reregister" RESET

// Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
# define ERR_PASSWDMISMATCH RED "464 :Password incorrect" RESET

// Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_NOPRIVILEGES RED "481 :Permission Denied- You're not an IRC operator" RESET

// Indicates that no client can be found for the supplied nickname.
# define ERR_NOSUCHNICK(nickname) RED "401 " + nickname + " :No such nickname" RESET

// Indicates that no channel can be found for the supplied channel name.
# define ERR_NOSUCHCHANNEL(channel) RED "403 " + channel + " :No such channel" RESET

// Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send.
# define ERR_NOTEXTTOSEND RED "412 <client> :No text to send" RESET

// Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).
# define ERR_INPUTTOOLONG RED "417 <client> :Input line was too long" RESET

//Channel errors

// Returned when a client tries to invite a user to a channel they are already on.
# define ERR_USERONCHANNEL(user, channel) RED "443 " + user + channel + " :is already on channel" RESET

// Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_TOOMANYCHANNELS(channel) RED "405 " + channel + " :You have joined too many channels" RESET

# define ERR_CHANNELISFULL(channel) RED "471 " + channel + " :Cannot join channel (+l)" RESET

# define ERR_UNKNOWNMODE(char) RED "472 " + char + " :is unknown mode char to me" RESET

# define ERR_INVITEONLYCHAN(channel) RED "473 " + channel + " :Cannot join channel (+i)" RESET

// Returned when the "join #channel" syntax is not respected
# define ERR_INVCHANNELNAME	RED "The channel name must be preceded by an '#' to be interpreted" RESET

#endif
