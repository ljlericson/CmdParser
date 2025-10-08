# S3DL-3.0

This is yet another remake of s3dl, I'm hoping to write clean and efficient code that doesn't get overcomplicated like s3dl 2.0. I am going to use a build system like cmake with this project and also use different naming conventions as something new to do.



### How To Build
Please note this is a windows only project. 
1. Clone the repository with `git clone https://github.com/ljlericson/CmdParser --recurse-submodules`
2. Navigate into the vcpkg dir and run `bootstrap-vcpkg.bat`
3. Open a command prompt in the vcpkg dir and run `vcpkg install` (IMPORTANT!! This will take around 20-30 minutes as it will download and build the dependencies for this project)
4. In the parent repo, run "generate.bat"
5. Open a terminal in the new build forder and run `make` or `mingw32-make`
8. Build and run...