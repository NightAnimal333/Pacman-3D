# El Pacmaño

A first person pacman game with working score and lives.

## Creators
- Artūrs Umbraško
- Kacper Lewandowski

## Setup instruction
**In most cases** you will just need to run either Linux or Windows executables, that are already in the repo.



**If you want to compile yourself** then read on. The easiest way is to just run the compile scripts that are already in the repo.

Setup of the project should be easy. Simply use CMake on the CMakeLists.txt in the root folder of repository.
To make the game work properly, you also need to:
- (Windows) You also have to copy irrKlang.dll and ikpMP3.dll from "{project path}/external/irrKlang/bin/winx64-visualStudio" into "{project path}/out/build/x64-Debug" AFTER running CMake.
- (Ubuntu) You also have to copy irrKlang.dll and ikpMP3.dll from "{project path}/external/irrKlang/bin/linux-gcc" into the project folder.

In case this doesn't work properly for some reason, you can:
- (Ubuntu) Use G++ and link all .cpp and .so files and include .h files properly. A compile bash script that I used is provided. Up to date Ubuntu compiled binary is also provided, just in case. 
- (Ubuntu) In case you are not going to use the file provided, you most likely will have to add "export MESA_GL_VERSION_OVERRIDE=4.3" and "export LD_LIBRARY_PATH=./" before compiling and running.
- Write an issue or send a message to repo owner


## Ingame controls
Press on the start game button and navigate Pacman using arrow buttons **NOT WASD** and mouse.
You can enter pause menu again anytime by pressing ESC. You can also entirely quit the game by hitting "Q" while in-game.

## Implemented features
- Fully functioning levels and tilemap
- New maps can be created by adding "map$.txt" where $ is the new map number, and changing one value in main.cpp
- Ghosts move and can kill pacman
- Pacman collects points 
- GUI that shows lives, score, and has a main menu, pause menu and settings
- Configurable settings include volume and render distance.
- Settings are persistent as they are saved as file.
- The game has music and sounds effects!
- Ghosts, walls, tiles and chilis have a texture
- Ghosts, walls are imported from OBJ file.
- Tiles and chilis are sprites to improve performance
- Mouse camera controls


## Unimplemented features
- There is no minimap
- Lighting system is only ambient light
- Can't navigate main menu with buttons

## Currently known bugs
- Pacman can clip through the wall, he still won't be able to go through them.
- Be the one to find bugs!
