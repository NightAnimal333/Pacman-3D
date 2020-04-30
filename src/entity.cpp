#include "entity.h"

Entity::~Entity() {
	/*
	I know this is a bit unorthodox way of detroying objects
	But this prevents OpenGL from deleting all the stuff
	 whenever a temporary entity is created, for example in Loader::load()
	Comments and suggestions about this would be appreciated
	*/
	if (!trueDestroy) {
		return;
	}
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->VAO);

	uniqueVertices.clear();
	meshes.clear();
}

//---------------------
//OPENGL only functions
//---------------------

void Entity::initialise() {
	//Generate VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//Generate VBO
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, uniqueVertices.size() * sizeof(Vertex), this->uniqueVertices.data(), GL_STATIC_DRAW);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].initialiseEBO();
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Entity::addUniqueVertices(std::vector<Vertex> newUniqueVertices) {
	this->uniqueVertices = newUniqueVertices;
}

void Entity::addMesh(Mesh mesh) {
	this->meshes.push_back(mesh);
}

void Entity::resize(glm::vec3 newScale) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].resize(newScale);
	}
}

void Entity::rotate(float radians, char axis) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].rotate(radians, axis);
	}
}

void Entity::clearRotation() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].clearRotation();
	}
}

void Entity::move(float distance, char axis) {
	switch (axis) {
	case 'x':
		this->position.x += distance;
		break;

	case 'y':
		this->position.y += distance;
		break;

	case 'z':
		this->position.z += distance;

		break;
	}

	//Apply for all meshes
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].move(distance, axis);
	}
}


void Entity::setPosition(glm::vec3 newPosition) {
	this->position = newPosition;
}

glm::vec3 Entity::getPosition() {
	return this->position;
}

void Entity::draw(GLuint shader) {
	glUseProgram(shader);

	//Bind VAO
	glBindVertexArray(this->VAO);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
}

//--------------
//GAME FUNCTIONS
//--------------

void Entity::moveAI(double delta) {
	GLfloat distance = 2.f * delta;		//Speed of ghosts
	char axis;							//Which direction (axis) are they supposed to walk on

	switch (this->direction) {
	case 0:
		this->position.z += distance;
		axis = 'z';
		break;

	case 1:
		this->position.x += distance;
		axis = 'x';
		break;

	case 2:
		distance *= -1;
		this->position.z += distance;
		axis = 'z';
		break;

	case 3:
		distance *= -1;
		axis = 'x';
		this->position.x += distance;
		break;
	}

	//Apply for all meshes
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].move(distance, axis);
	}
}

void Entity::teleportToMiddle() {
	float differenceX = 0.5f - fmod(position.x, 1);	//Gets ghosts decimal value away from the middle of a tile.
	float differenceZ = 0.5f - fmod(position.z, 1);	//Same here but Z axis


	//These two if statements make sure the ghost does not hug walls
	//They check if the ghost is far away from the middle
	//If he is, he gets teleported into the middle

	if (differenceX < -0.1 || differenceX > 0.1f) {
		position.x = int(position.x) + 0.5f;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].move(differenceX, 'x');
		}
	}

	if (differenceZ < -0.1 || differenceZ > 0.1f) {
		position.z = int(position.z) + 0.5f;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].move(differenceZ, 'z');
		}
	}
}

glm::vec3 Entity::getTargetLocation() {
	return this->targetLocation;
}

int Entity::getDirection() {
	return this->direction;
}

void Entity::setTargetLocation(glm::vec3 newTargetLocation) {
	this->targetLocation = newTargetLocation;
}

void Entity::setDirection(int newDirection) {
	this->direction = newDirection;
}

//Set the object to be wiped from GPU on next destructor call
void Entity::imminentOblivion() {
	trueDestroy = true;
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].imminentOblivion();
	}
}
