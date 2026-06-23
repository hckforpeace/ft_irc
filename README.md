# ft_irc

A lightweight IRC server written in C++98, compatible with standard IRC clients. This is the [42 School](https://42.fr) `ft_irc` project: build an Internet Relay Chat server from scratch using only low-level networking primitives.

The server handles multiple clients simultaneously over a single thread using `epoll`-based non-blocking I/O, supports authentication, nicknames, channels with operators, and the core set of IRC commands needed to chat, moderate, and manage channels.

## Features

- **Single-threaded, non-blocking I/O** — all connections are multiplexed with `epoll`; no polling loops, no threads.
- **Password-protected access** — clients must authenticate before registering.
- **User registration** — nickname and username handling with collision detection.
- **Channels** — join, leave, and message channels, with operator privileges.
- **Channel operators & moderation** — kick, invite, set topic, and change channel modes.
- **Channel modes** — invite-only (`i`), topic restriction (`t`), key/password (`k`), operator (`o`), and user limit (`l`).
- **Private and channel messaging** — `PRIVMSG` to users and channels.
- **Standard IRC replies** — numeric reply codes so real IRC clients work out of the box.

## Requirements

- A C++ compiler with C++98 support (`c++` / `g++` or `clang++`)
- `make`
- Linux (the server relies on `epoll`)

## Getting started

Clone the repository and build the server:

```bash
make
```

This produces the `ircserv` executable.

### Usage

```bash
./ircserv <port> <password>
```

| Argument     | Description                                          |
| ------------ | ---------------------------------------------------- |
| `port`       | TCP port the server listens on (`0`–`65535`)         |
| `password`   | Connection password every client must provide        |

Example:

```bash
./ircserv 6667 secretpass
```

> [!NOTE]
> The server binds to `127.0.0.1` (localhost), so connect from the same machine.

### Connecting

Use any standard IRC client. With [irssi](https://irssi.org/):

```bash
irssi
/connect 127.0.0.1 6667 secretpass
/nick alice
/join #general
```

Or test raw with `nc`:

```bash
nc 127.0.0.1 6667
PASS secretpass
NICK alice
USER alice 0 * :Alice
JOIN #general
PRIVMSG #general :Hello, world!
```

## Supported commands

| Command   | Description                                              |
| --------- | ------------------------------------------------------- |
| `PASS`    | Authenticate with the server password                   |
| `NICK`    | Set or change nickname                                   |
| `USER`    | Register username and real name                          |
| `JOIN`    | Join a channel (creating it if needed)                  |
| `PART`    | Leave a channel                                          |
| `PRIVMSG` | Send a message to a user or channel                     |
| `TOPIC`   | View or set a channel topic                              |
| `INVITE`  | Invite a user to a channel                               |
| `KICK`    | Remove a user from a channel (operators only)           |
| `MODE`    | Change channel modes (`i`, `t`, `k`, `o`, `l`)          |
| `PING`    | Keepalive; replies with `PONG`                          |
| `QUIT`    | Disconnect from the server                               |

### Channel modes

| Mode | Effect                                                        |
| ---- | ------------------------------------------------------------ |
| `+i` | Invite-only — users can only join if invited                 |
| `+t` | Only operators can change the topic                          |
| `+k` | Set a channel key (password) required to join                |
| `+o` | Grant or revoke operator status to a user                    |
| `+l` | Set a maximum number of users allowed in the channel         |

Prefix a mode with `-` to remove it, e.g. `MODE #general -i`.

## Project structure

```
ft_irc/
├── includes/        # Header files (Server, Client, Channel, Replies, ...)
├── srcs/            # Implementation
│   ├── main.cpp           # Entry point, argument parsing
│   ├── Server.cpp         # Socket setup, epoll loop, dispatch
│   ├── Client.cpp         # Client state
│   ├── Channel.cpp        # Channel state
│   ├── basic_commands.cpp # PASS, NICK, USER, PRIVMSG, ...
│   ├── Join.cpp           # JOIN handling
│   ├── Mode.cpp           # MODE handling
│   ├── Channel_Commands.cpp # KICK, INVITE, TOPIC, PART
│   └── utils.cpp          # Helpers
└── Makefile
```

## Make targets

| Target    | Description                                  |
| --------- | -------------------------------------------- |
| `make`    | Build `ircserv`                              |
| `clean`   | Remove object files                          |
| `fclean`  | Remove object files and the executable       |
| `re`      | Full rebuild (`fclean` + `all`)              |
