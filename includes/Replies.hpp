#ifndef REPLIES_HPP
# define REPLIES_HPP

// Command responses

// Error replies

//Returned to a registered client to indicate that the command sent is unknown by the server.
# define ERR_UNKNOWNCOMMAND(command) "421 " + command + " :Unknown command\n"

// Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOTREGISTERED "451 : You have not registered"

// Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
# define ERR_NEEDMOREPARAMS(command) "461 " + command + " :Not enough parameters\n"

// Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
# define ERR_ALREADYREGISTRED(user) "462 " + user + " :You may not reregister\n"

// Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
# define ERR_PASSWDMISMATCH "464 :Password incorrect\n"

// Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_NOPRIVILEGES "481 :Permission Denied- You're not an IRC operator\n"

//Channel errors

// Returned when a client tries to invite a user to a channel they are already on.
# define ERR_USERONCHANNEL(user, channel) "443 " + user + channel + " :is already on channel\n"

// Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_TOOMANYCHANNELS(channel_name) "405 " + channel_name + " :You have joined too many channels\n"

# define ERR_CHANNELISFULL(channel) "471 " + channel + " :Cannot join channel (+l)\n"

# define ERR_UNKNOWNMODE(char) "472 " + char + " :is unknown mode char to me\n"

# define ERR_INVITEONLYCHAN(channel) "473 " + channel + " :Cannot join channel (+i)\n"


#endif
