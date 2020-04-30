#include <iostream>
#include <string>

#include "mesh.h"

Mesh::~Mesh() {
	//Checks if the mesh really should die.
	if (!trueDestroy) {
		return;
	}

	glDeleteBuffers(1, &this->EBO);
	vertexArray.clear();
	indexArray.clear();
}

Mesh::Mesh(std::vector<GLuint> newIndexArray) {
	this->indexArray = newIndexArray;
	trueDestroy = false;

	position = glm::vec3(0.f, 0.f, 0.f);
	origin = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec3(0.f, 0.f, 0.f);
	scale = glm::vec3(1.f);
}

void Mesh::initialiseEBO() {

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(GLuint), this->indexArray.data(), GL_STATIC_DRAW);
}


void Mesh::setTexture(Texture newTexture) {
	this->texture = newTexture;
}

void Mesh::updateMatrix() {
	this->modelMatrix = glm::mat4(1.f);
	this->modelMatrix = glm::translate(this->modelMatrix, this->origin);	//Moves model
	//Rotates model in respecting axis
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	this->modelMatrix = glm::scale(this->modelMatrix, this->scale);			//Scales model
}

void Mesh::rotate(float radians, char axis) {
	switch (axis) {
	case 'x':
		this->rotation.x += radians;
		break;

	case 'y':
		this->rotation.y += radians;
		break;

	case 'z':
		this->rotation.z += radians;
		break;
	}
}

void Mesh::clearRotation() {
	this->rotation = glm::vec3(0.f);
}

void Mesh::move(float distance, char axis) {
	switch (axis) {
	case 'x':
		this->origin.x += distance;
		break;

	case 'y':
		this->origin.y += distance;
		break;

	case 'z':
		this->origin.z += distance;

		break;
	}
}

void Mesh::resize(glm::vec3 newScale) {
	this->scale = newScale;
}

void Mesh::draw(GLuint shader) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	this->updateMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindTexture(GL_TEXTURE_2D, texture.getID());

	glDrawElements(GL_TRIANGLES, this->indexArray.size(), GL_UNSIGNED_INT, 0);
}

//Set the object to be wiped from GPU on next destructor call
void Mesh::imminentOblivion() {
	trueDestroy = true;

	texture.imminentOblivion();
}
