#pragma once

#ifndef __MANAGER_H
#define __MANAGER_H

#include <vector>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "loader.h"
#include "entity.h"


/*Used to create an abstract map of level
w = wall, p = pizza, P = pacman, g = ghost, e = empty */
typedef enum { w, p, P, g, e } tile;

//Size of the levels
int const SIZE_X = 28;
int const SIZE_Z = 36;


//-----------------------
//Map based tile functions
//-----------------------

//Loads the map from a file
void loadMap(const char* source);

//Returns middle of a z and x position of tile with a specified number
//For example findCoordinates(4,12) will return (4.5, 0, 12.5)
std::vector<GLfloat> findCoordinates(unsigned int x, unsigned int z);

//Find what kind of enum specified number has
tile findTile(GLfloat x, GLfloat z);

//Spawns all objects into the map
glm::vec3 spawn(Loader *loaderSingleton, std::vector<Entity> *walls, std::vector<Entity> *ghosts, std::vector<Entity> *pizzas, std::vector<Entity> *tiles);


//-----------------------
//Logic based tile functions
//-----------------------

//Checks whether an area around of CAMERAMAN is taken
std::vector<bool> checkIfWall(glm::vec3 cameraPos, glm::vec3 cameraFace);

//Decides in which direction the ghost is supposed to move in and check if ghost has reached target location
void moveGhost(Entity *ghost);

//Checks if CAMERAMAN is on the same tile as a pizza, if is then return true
bool eatPizza(glm::vec3 pacman, Entity& pizza);

//Checks if CAMERAMAN is on the same tile as ghost, if is then return true
bool pacmanBusted(glm::vec3 pacman, Entity& ghost);



#endif
