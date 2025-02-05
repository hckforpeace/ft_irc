#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>

class Parser 
{
  private:
    std::string message;
    std::string cmd;
    std::string parameters;
    std::string nick;
  public:
    Parser(std::string message); 

};

#endif // !PARSER_HPP
