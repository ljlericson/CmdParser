#include "include/ljl/Cmd.hpp"
#include <iostream>


int main(int argc, char** argv) 
{
    std::ifstream file("args.json");
    if(!file.is_open()) 
    {
        std::cerr << "Error opening JSON file \n";
        return -1;
    }
    nlohmann::json json;
    file >> json;

    // make a cmd object and pass in the json object
    ljl::cmd command{argc, argv, json};
    
    // now we check which type of command it is:
    // queries are commands that have predermined answers
    // for example: --version might return "v0.01" or
    // --help might return a list of commands
    if(command.is(ljl::cmd::type::query))
    {
        // after knowing it is a query call this function
        // to respond. 
        // NOTE: this function will do nothing if you
        // haven't called ".is()" prior to calling this
        // function

        command.respond();
    }
    // "commands" are the second type of command and are
    // functions of the program that have any other
    // functionality aside from simple printing
    if(command.is(ljl::cmd::type::command))
    {
        // this is an example of a command with customs args
        // and how the values of arguments are collected 
        if(command["run"])
        {    
            // NOTE: only ints/uints, float/double and std::string are supported
            std::cout << command.get_value<float>("run", "<speed>") << '\n';
            std::cout << command.get_value<std::string>("run", "<type>") << '\n';
        }
        // this is a command so custom functionality is assumed
        // but no args are collected.
        if(command["hello"])
        {
            std::cout << "Custom command hello!\n";
        }
    }

    return 0;
}