# The project
This project is a game engine for RPG like games. It takes a bunch of files and execute the game described in it.
The engine will come with a executable to create the game files.

# Build
## Dependencies
To build the project, you will need:
- cmake (> 3.10)
- g++ (>= 8, needed for c++17 features)
- doxygen (documentation)

To build the project, you need to pull the third-parties libraries. They are in git submodules:
```
git submodule update --init --recursive
```

## Configuration
Here are the CMake variables:
| Variable | Type | Description | Default |
| ------ | ------ | ------ | ------ |
| RPG_BUILD_GUI | Bool | Enable the building of the GUI part of the engine | ON |
| RPG_BUILD_TEST | Bool | Enable the build of tests. Disable this if you do not want to build the tests and googletest dependency | ON |
| RPG_COMPILE_THIRDPARTIES | Bool | Enable the build of the thirdparties. | ON |
| RPG_VERBOSE | Number | Set the level of the verbosity (see Config/VerbosityLevel.hpp) | 10 |


## Building
To build the project, execute these commands :
```
export RPG_REPOSITORY=`pwd`
cd ..
mkdir build
cd build
cmake ${RPG_REPOSITORY}
make
```

## Documentation
You can build the documentation with `make doc` when you are in the build directory. You can use `make show_dow` to open the html in you browser.
