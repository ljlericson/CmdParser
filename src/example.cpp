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

    // make a cmdparser object and pass in the json object
    ljl::cmdparser cmdparser{argc, argv, json};
    
    // now we check which type of command it is:
    // queries are commands that have predermined answers
    // for example: --version might return "v0.01" or
    // --help might return a list of commands
    if(cmdparser.is(ljl::cmdparser::type::query))
    {
        // after knowing it is a query call this function
        // to respond. 
        // NOTE: this function will do nothing if you
        // haven't called ".is()" prior to calling this
        // function

        cmdparser.respond();
    }
    // "commands" are the second type of command and are
    // functions of the program that have any other
    // functionality aside from simple printing
    if(cmdparser.is(ljl::cmdparser::type::command))
    {
        // this is an example of a explicit passtype command 
        // with customs argsand how the values of arguments 
        // are collected 
        if(cmdparser["run"])
        {    
            // NOTE: only ints/uints, float/double and std::string are supported
            std::cout << cmdparser.get_value<float>("run", "-speed") << '\n';
            std::cout << cmdparser.get_value<std::string>("run", "-type") << '\n';

            /* EXAMPLE USE CASE:
            application->run(command.get_value<float>("run", "-speed"),
                             command.get_value<std::string>("run", "-type"));
                example usage: CmdParser.exe run2 -speed 123 -type coolType
            */
        }
        // this is an example of an command with an implicit passtype
        if(cmdparser["run2"])
        {    
            // this performs the same task as the first example
            std::cout << cmdparser.get_value<float>("run2", "<speed>") << '\n';
            std::cout << cmdparser.get_value<std::string>("run2", "<type>") << '\n';
            // example usage: CmdParser.exe run2 123 coolType
        }
        // this is a command so custom functionality is assumed
        // but no args are collected.
        if(cmdparser["hello"])
        {
            std::cout << "Custom command hello!\n";
        }
    }

    return 0;
}