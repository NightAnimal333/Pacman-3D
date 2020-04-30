#pragma once

#ifndef __ENTITY_H
#define __ENTITY_H

#include <iostream>
#include <vector>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "vertex.h"
#include "mesh.h"

class Entity {
private:
	//EVERY ENTITY

	//Contains all meshes for an entity.
	std::vector<Mesh> meshes;
	//Contains all vertex objects as a big vector
	std::vector<Vertex> uniqueVertices;

	//Each entity has a VAO and a VBO, whereas EBO is controlled by individual meshes
	GLuint VAO;
	GLuint VBO;

	glm::vec3 position = glm::vec3(0.f);

	//Used by destructor
	bool trueDestroy = false;

	//GHOST ONLY
	//Stores where the entity is going to
	glm::vec3 targetLocation;
	//Stores the direction in which ghost is heading
	int direction = -1;

public:
	Entity() = default;
	~Entity();

	//-----------------------
	//OpenGL specific functions
	//-----------------------

	void initialise();

	//Adds unique vertices to the entity. Used only in LOADER
	void addUniqueVertices(std::vector<Vertex> newUniqueVertices);

	//Adds mesh to the entity. Used only in LOADER
	void addMesh(Mesh mesh);

	//Resizes all meshes for an entity. Used only in MANAGER
	void resize(glm::vec3 newScale);

	//Rotates all meshes for an entity. Used only in MANAGER
	void rotate(float radians, char axis);

	//Sets rotation to 0 for all meshes in an entity. Used only by GHOST in MANAGER
	void clearRotation();

	//Moves an entity and all its meshes.
	void move(float distance, char axis);

	//Sets position for all meshes for an entity
	void setPosition(glm::vec3 newPosition);

	//Gets position of the entity
	glm::vec3 getPosition();

	//Draws all meshes for an entity
	void draw(GLuint shader);

	//-----------------------
	//Game specific functions
	//-----------------------

	//Moves GHOST depending on the direction specified. Used in MAIN
	void moveAI(double delta);

	//Teleports GHOST to the middle of a tile (to avoid having ghost inside of a wall). Used in only MANAGER
	void teleportToMiddle();

	//Checks if GHOST has reached his destination. Used only in MANAGER
	glm::vec3 getTargetLocation();

	//Retrives direction of a ghost. Used in MANAGER
	int getDirection();

	//Sets direction of a ghost. Used only in MANAGER
	void setDirection(int newDirection);

	//Sets where the ghost is supposed to go. Used only in MANAGER
	void setTargetLocation(glm::vec3 newTargetDirection);

	//Call to destory an entity completely and wipe it from OpenGL
	void imminentOblivion();
};

#endif
