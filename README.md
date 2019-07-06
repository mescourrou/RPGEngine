#Presentation

The project is about creating a Role Play Game engine to be able to play the game described by all the files given.

# Building

## Dependencies
You will need:
- CMake
- A compiler supporting c++17 features
- git


## Setup the repository
```
git clone https://github.com/mescourrou/RPGEngine.git
cd RPGEngine
git submodule init
git submodule update --recursive
```

## Building
Here are the CMakes options available:
| CMake Option    | Default Value  | Description                                    |
|-----------------|----------------|------------------------------------------------|
| RPG_BUILD_TEST  | ON             | Build the tests                                |
| RPG_BUILD_GUI   | ON             | Build the GUI classes and dependencies         |
| RPG_COMPILE_THIRDPARTIES | ON    | Build the thirdparties libs   		    |
| RPG_VERBOSE     | 10             | Default level of verbosity                     |
| BUILD_USE_FILESYSTEM_PATH | ON   | Disable this if you have issues with std::filesystem::path |
| GAME_AUTO_CHOOSE| ON             | Choose automatically the 1st game in the list  |
| GAME_DEFAULT_RESOLUTION|"800x600"| Default resolution of the window               |

To build the project, proceed as follow:
```
mkdir build
cmake ..
make -j
```
Be sure to have pull the git submodules

# Installation
No installation for now, the executable is in the build directory, build/bin/Engine/RPGEngine.

