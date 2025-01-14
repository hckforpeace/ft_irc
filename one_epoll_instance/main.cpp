#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
int main()
{
	std::cout << "Run this program Twice on two different terminals, to stop CTRL+C" << std::endl;
	int epoll_fd = epoll_create(3);
	std::cout << "epoll_fd: " << epoll_fd << std::endl;
	while (1)
	{}
	if (close(epoll_fd) < 0)
		std::cout << "epoll_fd failed to close" << std::endl;
	else
		std::cout << "epoll successfuly closed" << std::endl;
	return (0);
}
