# CmdParser

CmdParser is a tool I made for parsing command line arguments in c++


### How To Build
Please note that this repo is a library that you can simple copy and paste into a current project.
These building instruction are for how to build the example I have given, using vcpkg for nlohman::json dependency.
1. Clone the repository with `git clone https://github.com/ljlericson/CmdParser --recurse-submodules`
2. Navigate into the vcpkg dir and run `bootstrap-vcpkg.bat`
3. Open a command prompt in the vcpkg dir and run `vcpkg install` (IMPORTANT!! This will take around 20-30 minutes as it will download and build the dependencies for this project)
4. In the parent repo, run "generate.bat"
5. Open a terminal in the new build forder and run `make` or `mingw32-make`
8. Build and run...