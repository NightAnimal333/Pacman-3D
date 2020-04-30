#pragma once

#ifndef __MESH_H
#define __MESH_H

#include <vector>
#include <string>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.h"
#include "texture.h"

class Mesh {

private:

	std::vector<Vertex> vertexArray;
	std::vector<GLuint> indexArray;

	//Currently unused
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint EBO;

	Texture texture;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	//Used by destructor
	bool trueDestroy;

public:

	Mesh() = default;
	Mesh(std::vector<GLuint> newIndexArray);

	~Mesh();


	void initialiseEBO();

	//Sets texture for specific mesh
	void setTexture(Texture newTexture);

	//Update the position, rotation and scale of a mesh
	void updateMatrix();

	//Sets a rotation based on degrees
	void rotate(float radians, char axis);

	//Sets rotation matrix back to 0
	void clearRotation();

	//Move the mesh a distance away in axis. The axis can be x, y or z.
	void move(float distance, char axis);

	//Scale up or down a model.
	void resize(glm::vec3 newScale);

	//Draw the model itself
	void draw(GLuint shader);

	//Call to destory an object completely and wipe it from OpenGL
	void imminentOblivion();
};

#endif
