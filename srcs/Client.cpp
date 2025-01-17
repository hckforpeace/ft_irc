#include "Client.hpp"

Client::Client(int fd):fd(fd)
{

}

Client::~Client()
{
    std::cout << "Client with fd" << fd << " destroyed" << std::endl;
    close(fd);
}
