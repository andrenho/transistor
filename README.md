# New project template: C++, SDL, Lua, ImGUI

![build](https://github.com/andrenho/template-cpp-sdl-lua/actions/workflows/build-and-deploy.yml/badge.svg)

A template for new C++ projects. Meant mostly for games but can be used for anything else.

It includes:
  - Cmake configuration files
  - Pre-configured set of C++ warnings
  - Automatically download git submodules
  - Create and run a SDL environment
    - Optionally embed SDL resources in final binary
    - ImGUI support
  - Lua library
    - Optionally embed Lua scripts in final binary
  - valgrind leak checking
  - github actions (pushing a new tag named 'v*' will create a new release and upload the source package).
   
To start:
  - Change project name and version in `CMakeLists.txt`
  - Remove sections that are not going to be used
  - Update the LICENSE file, if necessary
  - Rewrite this README
    - Instruct users to run `./build.sh` first

CMake targets:
  - `install`: install application on disk
  - `package_source`: create tarball of application
  - `leaks`: run the application and check for leaks with valgrind
  - `leaks-gen-suppression`: generate new suppression file for valgrind
