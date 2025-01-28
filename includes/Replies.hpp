#ifndef REPLIES_HPP
# define REPLIES_HPP

/*================================================ CHANNEL ===================================================*/

// Welcome message when a client joins a channel
# define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel

// Returned when a client tries to invite a user to a channel they are already on.
# define ERR_USERONCHANNEL(user, channel) ("443 :" + user + " is already on channel #" + channel)

# define ERR_UNKNOWNMODE(char) "472 " + char + " :is unknown mode char to me"

# define ERR_INVITEONLYCHAN(channel) "473 " + channel + " :Cannot join channel (+i)"

# define ERR_USERNOTONCHAN(nickname, channel) (":localhost 442 " + nickname + " #" + channel + " :You're not on that channel")

// Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_TOOMANYCHANNELS(client) ("localhost: 405 " + client + ": You have joined too many channels")

/*================================================= JOIN ======================================================*/

# define RPL_JOIN(hostname, channel) ":" + hostname + "@127.0.0.1 JOIN #" + channel + "\r\n"

# define RPL_NAMES(nickname, channel, clientslist) (": 353 " + nickname + " @ #" + channel + " :" + clientslist + "\r\n")

# define RPL_ENDOFNAMES(nickname, channel) (": 366 " + nickname + " #" + channel + " :END of /NAMES list")

/*================================================== PART ====================================================*/

#define RPL_PART(nickname, username, channel) (":" + nickname + "!~" + username + "@127.0.0.1 PART #" + channel)

/*================================================= TOPIC ====================================================*/

#define RPL_TOPIC(nickname, channel, topic) (": 332 " + nickname + " #" + channel + " :" + topic + "\r\n")

# define EMPTY_TOPIC(channel) ("#" + channel + ": No topic is set")

# define SHOW_TOPIC(channel, topic, client) ("Topic for #" + channel + ": " + topic + "\nTopic set by " + client)

# define CHANGE_TOPIC(client, channel, topic) (client + " changed the topic of #" + channel + " to: " + topic)

/*================================================ ERRORS =======================================================*/

//Returned to a registered client to indicate that the command sent is unknown by the server.
#define ERR_UNKNOWNCOMMAND(nickname, command) (":localhost 421 " + nickname + " " + command + " :Unknown command")

// Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOTREGISTERED(nickname) (":localhost 451 " + nickname + " : You have not registered")

// Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
# define ERR_NEEDMOREPARAMS(nickname) (":localhost 461 " + nickname + " :Not enough parameters")

// Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
# define ERR_ALREADYREGISTRED(user) (":localhost 462 " + user + " :You may not reregister")

# define ERR_PASSWORDNOTINSERTED "42? :You didn't insert the password\n" //????????

// Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
# define ERR_PASSWDMISMATCH(nickname) (":localhost 464 " + nickname + " :Password incorrect")

// Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_NOPRIVILEGES(nickname) (":localhost 481 " + nickname + " :Permission Denied - You're not an IRC operator")

// Indicates that no client can be found for the supplied nickname.
# define ERR_NOSUCHNICK(nickname) (":localhost 401 " + nickname + " :No such nickname")

# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 " + nickname + " :Nickname is already in use")

# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 " + nickname + " :Erroneous nickname")

// Indicates that no channel can be found for the supplied channel name.
# define ERR_NOSUCHCHANNEL(nickname, channel) (":localhost 403 " + nickname + " " + channel + " :No such channel\r\n")

// Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send.
# define ERR_NOTEXTTOSEND (":localhost 412 <client> :No text to send") //????????????

// Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).
# define ERR_INPUTTOOLONG (":localhost 417 <client> :Input line was too long") //?????????????????

/*====================================================== INVITE ============================================================*/

# define INVITE_ONLY(channel) "473: Cannot join to channel #" + channel + " (You must be invited)"

# define BE_INVITED(client, channel) client + " invites you to "+ channel

# define TO_INVITE(who_invite, invited, channel) who_invite + " invited " + invited + " into channel " + channel

/*==================================================== MODES ===============================================================*/

// mode/#channel [+/-k pass] || mode/#channel [+/-o client] || mode/#channel [+l limit]
# define MODE_SET(channel, nickname, mode) "mode/#" + channel + " [+" + mode + "] by " + nickname

# define MODE_UNSET(channel, nickname, mode) "mode/#" + channel + " [-" + mode + "] by " + nickname

// Password
# define CHAN_PASS(channel) "Cannot join to channel #" + channel + " (Bad channel key)"

// Operator
# define ERR_NOTOPERATOR(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You're not channel operator")

// User limit
# define ERR_CHANNELISFULL(channel) "471: Cannot join to channel #" + channel + " (Channel is full)"

# define ERR_BADMODESYNTAX "Bad syntax, use as : mode #channel +/-c [args], where c is the mode"

# define ERR_BADMODE "Mode not handled, aviable modes are: i (invite), t (topic), k (password), o (operator), l (limit)"

#endif
