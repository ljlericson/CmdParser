#include "include/ljl/cmd.hpp"
#include <iostream>


int main(int argc, char** argv) 
{
    ljl::cmd command{argc, argv, "args.json"};
    
    if(command.is(ljl::cmd::CmdType::query))
    {
        command.respond();
    }
    if(command.is(ljl::cmd::CmdType::command))
    {
        if(command["run"])
        {    
            std::cout << command.get_value<int>("run", "<speed> (int)") << '\n';
            std::cout << command.get_value<std::string>("run", "<type> (string)") << '\n';
        }
    }

    return 0;
}