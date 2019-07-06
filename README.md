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

| CMake Option               | Default Value  | Description                                                |
|----------------------------|----------------|------------------------------------------------------------|
| RPG_BUILD_TEST             | ON             | Build the tests                                            |
| RPG_BUILD_GUI              | ON             | Build the GUI classes and dependencies                     |
| RPG_COMPILE_THIRDPARTIES   | ON             | Build the thirdparties libs   		                         |
| RPG_VERBOSE                | 10             | Default level of verbosity                                 |
| BUILD_USE_FILESYSTEM_PATH  | ON             | Disable this if you have issues with std::filesystem::path |
| GAME_AUTO_CHOOSE           | ON             | Choose automatically the 1st game in the list              |
| GAME_DEFAULT_RESOLUTION    | "800x600"      | Default resolution of the window                           |

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

# Installation
No installation for now, the executable is in the build directory, build/bin/Engine/RPGEngine.
