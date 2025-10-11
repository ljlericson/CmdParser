# CmdParser

CmdParser is a tool I made for parsing command line arguments in c++

### Example:
```c++
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
```

### How To Build Example
Please note that this repo is a library that you can simple copy and paste into a current project.
These building instruction are for how to build the example I have given, using vcpkg for nlohman::json dependency.
1. Clone the repository with `git clone https://github.com/ljlericson/CmdParser --recurse-submodules`
2. Navigate into the vcpkg dir and run `bootstrap-vcpkg.bat`
3. Open a command prompt in the vcpkg dir and run `vcpkg install` (IMPORTANT!! This will take around 20-30 minutes as it will download and build the dependencies for this project)
4. In the parent repo, run "generate.bat"
5. Open a terminal in the new build forder and run `make` or `mingw32-make`
8. Build and run...