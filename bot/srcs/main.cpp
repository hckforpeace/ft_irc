#include "bot.hpp"
#include "Colors.hpp"


int main (int argc, char *argv[])
{
  if (argc != 5)
    std::cout << RED << "WRONG INPUT EXPECTED" << RESET GREEN << " => ./bot port_number ip_addr nickname realname channel_name forbiden_words.."  << RESET << std::endl;  

  return (0);
}

