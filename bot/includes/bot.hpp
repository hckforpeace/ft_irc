#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <string>
#include <vector>

class Bot
{
  private:
    std::string username;
    std::string realname;
    std::string nickname;
    std::vector<std::string> forbidden_words;
    std::string channel_name;
  public:
    Bot();

};

#endif // DEBUG
