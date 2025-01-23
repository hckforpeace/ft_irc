#ifndef REPLIES_HPP
# define REPLIES_HPP

// Command responses

/*================ CHANNEL ====================*/

// Welcome message when a client joins a channel
# define CHAN_WELC(nickname, channel) GREEN + nickname + " has joined #"  + channel + RESET

/*================= ERRORS ===================*/

//Returned to a registered client to indicate that the command sent is unknown by the server.
# define ERR_UNKNOWNCOMMAND(command) RED "421 " + command + " :Unknown command" RESET

// Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOTREGISTERED(nickname) RED "451 " + nickname + " : You have not registered" RESET

// Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
# define ERR_NEEDMOREPARAMS(command) RED "461 " + command + " :Not enough parameters" RESET

// Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
# define ERR_ALREADYREGISTRED(user) RED "462 " + user + " :You may not reregister" RESET

# define ERR_PASSWORDNOTINSERTED "42 :You didn't insert the password\n"

// Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
# define ERR_PASSWDMISMATCH RED "464 :Password incorrect" RESET

// Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_NOPRIVILEGES RED "481 :Permission Denied- You're not an IRC operator" RESET

// Indicates that no client can be found for the supplied nickname.
# define ERR_NOSUCHNICK(nickname) RED "401 " + nickname + " :No such nickname" RESET

# define ERR_NICKNAMEINUSE(nickname) RED "433 " + nickname + " :Nickname is already in use" RESET

# define ERR_ERRONEUSNICKNAME(nickname) RED "432 " + nickname + " :Erroneus nickname" RESET 

// Indicates that no channel can be found for the supplied channel name.
# define ERR_NOSUCHCHANNEL(channel) RED "403 " + channel + " :No such channel" RESET

// Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send.
# define ERR_NOTEXTTOSEND RED "412 <client> :No text to send" RESET

// Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).
# define ERR_INPUTTOOLONG RED "417 <client> :Input line was too long" RESET

//Channel errors

// Returned when a client tries to invite a user to a channel they are already on.
# define ERR_USERONCHANNEL(user, channel) RED "443 :" + user + " is already on channel #" + channel + RESET

# define ERR_UNKNOWNMODE(char) RED "472 " + char + " :is unknown mode char to me" RESET

# define ERR_INVITEONLYCHAN(channel) RED "473 " + channel + " :Cannot join channel (+i)" RESET

// Returned when the "join #channel" syntax is not respected
# define ERR_INVCHANNELNAME	RED "The channel name must be preceded by an '#' to be interpreted" RESET

// Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_TOOMANYCHANNELS(client) RED "405 " + client + ": You have already joined 10 channels, you can't join any other one" RESET

/*========== INVITE =============*/
# define INVITE_ONLY(channel) RED "Cannot join to channel #" + channel + " (You must be invited)" + RESET

/*======================== MODES =============================*/

// mode/#channel [+/-k pass] || mode/#channel [+/-o client] || mode/#channel [+l limit]
# define MODE_SET(channel, nickname, mode) BLU "mode/#" + channel + " [+" + mode + "] by " + nickname + RESET
# define MODE_UNSET(channel, nickname, mode) YEL "mode/#" + channel + " [-" + mode + "] by " + nickname + RESET

// Password
# define CHAN_PASS(channel) RED "Cannot join to channel #" + channel + " (Bad channel key)" RESET

// Operator
# define ERR_NOTOPERATOR(channel) RED "#" + channel + " :You're not channel operator" RESET

// User limit
# define ERR_CHANNELISFULL(channel) RED "471: Cannot join to channel #" + channel + " (Channel is full)" RESET

# define ERR_BADMODESYNTAX RED "Bad syntax, mode cmd must be terminated by +/-c, where c is the mode" RESET

# define ERR_BADMODE RED "Mode not handled, aviable modes are: i (invite), t (topic), k (password), o (operator), l (limit)" RESET

#endif
