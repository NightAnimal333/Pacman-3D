#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include <glad.h>

#include "manager.h"
#include "loader.h"
#include "entity.h"

/*Used to create an abstract map of level
w = wall, p = pizza, P = pacman, g = ghost */
GLfloat spawnPosition[2];

//The abstract map - id est the level - itself
tile mapAbstract[SIZE_X][SIZE_Z] = {};

void loadMap(const char* source)
{
	std::ifstream text{ source };
	char c;
	int i = 0, j = 0;

	//Gets single character and checks what type it is
	while (text.get(c)) {
		if (c == 'w') {
			mapAbstract[i][j] = w;
			i++;
		}
		if (c == 'g') {
			mapAbstract[i][j] = g;
			i++;
		}
		if (c == 'p') {
			mapAbstract[i][j] = p;
			i++;
		}
		if (c == 'P') {
			mapAbstract[i][j] = P;
			i++;
		}
		if (c == 'e') {
			mapAbstract[i][j] = e;
			i++;
		}

		//The while loop is the same as:
		//for (int j = 0; j < 36; j++)
			// for (int i = 0; i < 28; i++)
		if (i > 27) {
			j++;
			i = 0;
		}
	}

	text.close();
}


std::vector<GLfloat> findCoordinates(unsigned int x, unsigned int z) {
	//Creates two floats in a vector.
	std::vector<GLfloat> coordinates;
	coordinates.push_back(0.0f);
	coordinates.push_back(0.0f);

	//Add the position on tile + half to get the middle of that tile
	coordinates[0] = float(x + 0.5f);
	coordinates[1] = float(z + 0.5f);

	return coordinates;
}

tile findTile(GLfloat x, GLfloat z) {
	return mapAbstract[int(floor(x))][int(floor(z))];
}

std::vector<bool> checkIfWall(glm::vec3 cameraPos, glm::vec3 cameraFace) {
	float fVar = 0.2f;	//	How far from sides can a wall be.
	float fFace = 2.0f;	//	How far from front/back can a wall be.
	std::vector<bool> walls = { false, false, false, false };	//Checks which sides have walls
	glm::vec3 normal = glm::normalize(glm::cross(cameraFace, glm::vec3(0.f, 1.f, 0.f)));

	//Front
	if (findTile(cameraPos.x + (cameraFace.x * fFace), cameraPos.z + (cameraFace.z * fFace)) == w) {
		walls[0] = true;
	}
	//Back
	if (findTile(cameraPos.x - (cameraFace.x * fFace), cameraPos.z - (cameraFace.z * fFace)) == w) {
		walls[2] = true;
	}
	//Right
	if (findTile(cameraPos.x + (normal.x * fVar), cameraPos.z + (normal.z * fVar)) == w) {
		walls[3] = true;
	}
	//Left
	if (findTile(cameraPos.x - (normal.x * fVar), cameraPos.z - (normal.z * fVar)) == w) {
		walls[1] = true;
	}

	return walls;
}

std::vector<unsigned int> findTileNum(GLfloat x, GLfloat z) {
	std::vector <unsigned int> tileCoordinates;
	tileCoordinates.push_back(0);
	tileCoordinates.push_back(0);

	tileCoordinates[0] = int(floor(x));
	tileCoordinates[1] = int(floor(z));

	return tileCoordinates;
}

glm::vec3 spawn(Loader* loaderSingleton, std::vector<Entity>* walls, std::vector<Entity>* ghosts, std::vector<Entity>* pizzas, std::vector<Entity>* tiles) {
	glm::vec3 spawnPosition = glm::vec3(0.f);

	Texture tempWallTexture;
	Texture tempGhostTexture;
	Texture tempPizzaTexture;
	Texture tempTileTexture;

	tempWallTexture.load("./res/textures/watermelon.png");
	tempGhostTexture.load("./res/textures/mexican.png");
	tempPizzaTexture.load("./res/textures/pepper.png");
	tempTileTexture.load("./res/textures/watermelon_red.png");

	for (int z = 0; z < SIZE_Z; z++) {
		for (int x = 0; x < SIZE_X; x++) {
			std::vector<GLfloat> coordinates = findCoordinates(x, z);

			//Spawning walls
			if (mapAbstract[x][z] == w) {
				Entity tempWall = loaderSingleton->load("./res/objects/cube.obj", tempWallTexture);
				tempWall.resize(glm::vec3(0.5f));
				tempWall.initialise();
				tempWall.move(coordinates[0], 'x');
				tempWall.move(coordinates[1], 'z');
				walls->push_back(tempWall);
			}

			//Spawning ghosts
			if (mapAbstract[x][z] == g) {
				Entity tempGhost = loaderSingleton->load("./res/objects/ghost.obj", tempGhostTexture);
				tempGhost.resize(glm::vec3(0.4f));
				tempGhost.initialise();
				tempGhost.move(coordinates[0], 'x');
				tempGhost.move(coordinates[1], 'z');
				ghosts->push_back(tempGhost);
			}

			//Spawning pizzas
			if (mapAbstract[x][z] == p || mapAbstract[x][z] == g) {
				Entity tempPizza = loaderSingleton->loadSprite(tempPizzaTexture);
				tempPizza.resize(glm::vec3(0.15f));
				tempPizza.initialise();
				tempPizza.move(coordinates[0], 'x');
				tempPizza.move(coordinates[1], 'z');
				tempPizza.move(-0.4f, 'y');
				pizzas->push_back(tempPizza);
			}

			//Spawning tiles
			if (mapAbstract[x][z] != w && mapAbstract[x][z] != e) {
				Entity tempTile = loaderSingleton->loadSprite(tempTileTexture);
				tempTile.initialise();
				tempTile.move(coordinates[0], 'x');
				tempTile.move(coordinates[1], 'z');
				tempTile.move(-0.5f, 'y');
				tempTile.move(-0.5f, 'z');
				tempTile.move(-0.5f, 'x');
				tempTile.rotate(90.f, 'x');
				tiles->push_back(tempTile);
			}

			//Setting spawnpoint
			if (mapAbstract[x][z] == P) {
				spawnPosition.x = x + 0.5f;
				spawnPosition.y = 0.f;
				spawnPosition.z = z + 0.5f;
			}
		}
	}

	return spawnPosition;
}

void moveGhost(Entity* ghost) {
	//Random generator device for AI
	std::random_device seed{};
	std::mt19937 generator(seed());
	std::uniform_int_distribution<int> rng(0, 3);

	bool directions[4] = { false, false, false, false };
	int chosenDirection; int vacantDirections = 0;

	glm::vec3 ghostPosition = ghost->getPosition();

	int previousDirection = ghost->getDirection();

	//If not first loop
	if (!(previousDirection == -1)) {
		//Checking whether the ghost has reached next tile in his previously chosen direction
		switch (previousDirection) {
			//North
		case 0:
			if (ghostPosition.z < ghost->getTargetLocation().z) {
				return;
			}
			break;

			//East
		case 1:
			if (ghostPosition.x < ghost->getTargetLocation().x) {
				return;
			}
			break;

			//South
		case 2:
			if (ghostPosition.z > ghost->getTargetLocation().z) {
				return;
			}
			break;

			//West
		case 3:
			if (ghostPosition.x > ghost->getTargetLocation().x) {
				return;
			}
			break;
		}
	}

	//If the previous did not return - it means the ghost has reached next tile and needs a new direction
	ghost->teleportToMiddle();
	//Checking which tiles around are not walls
//North
	if (findTile(ghostPosition.x, ghostPosition.z + 1) != w) {
		directions[0] = true;
		vacantDirections++;
	}
	//East
	if (findTile(ghostPosition.x + 1, ghostPosition.z) != w) {
		directions[1] = true;
		vacantDirections++;
	}
	//South
	if (findTile(ghostPosition.x, ghostPosition.z - 1) != w) {
		directions[2] = true;
		vacantDirections++;
	}
	//West
	if (findTile(ghostPosition.x - 1, ghostPosition.z) != w) {
		directions[3] = true;
		vacantDirections++;
	}

	//If more than two tiles around are not wall or he's in a dead end
	if (vacantDirections >= 2 || vacantDirections == 1) {
		//Randomly choose a direction, that is not opposite of previous direction, except for dead ends
		do {
			chosenDirection = rng(generator);
		} while (!(directions[chosenDirection]) || ((((chosenDirection + 2) % 4) == previousDirection) && vacantDirections != 1));
		ghost->setDirection(chosenDirection);

		//Set target location to middle of next chosen tile
			//North
		switch (chosenDirection) {
		case 0: {
			ghost->setTargetLocation(glm::vec3(ghostPosition.x, 0.f, ghostPosition.z + 1));
			ghost->clearRotation();
			ghost->rotate(-90.f, 'y');
			break;
		}
			  //East
		case 1: {
			ghost->setTargetLocation(glm::vec3(ghostPosition.x + 1, 0.f, ghostPosition.z));
			ghost->clearRotation();
			break;
		}
			  //South
		case 2: {
			ghost->setTargetLocation(glm::vec3(ghostPosition.x, 0.f, ghostPosition.z - 1));
			ghost->clearRotation();
			ghost->rotate(90.f, 'y');
			break;
		}
			  //West
		case 3: {
			ghost->setTargetLocation(glm::vec3(ghostPosition.x - 1, 0.f, ghostPosition.z));
			ghost->clearRotation();
			ghost->rotate(180.f, 'y');
			break;
		}
		}
	}
}

//Checks whether pacman is standing on a pizza and eats it if he does
bool eatPizza(glm::vec3 pacman, Entity& pizza) {
	glm::vec3 checkPizza = pizza.getPosition();

	if ((floor(pacman.x) == floor(checkPizza.x)) && (floor(pacman.z) == floor(checkPizza.z))) {
		return true;
	}

	return false;
}

//Checks whether pacman is standing on the same tile as a ghost and busts him if he does
bool pacmanBusted(glm::vec3 pacman, Entity& ghost) {
	glm::vec3 checkGhost = ghost.getPosition();

	if ((floor(pacman.x) == floor(checkGhost.x)) && (floor(pacman.z) == floor(checkGhost.z))) {
		return true;
	}

	return false;
}
