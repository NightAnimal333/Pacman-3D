#define GLFW_DLL

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <irrKlang.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "vertex.h"
#include "mesh.h"
#include "entity.h"
#include "loader.h"
#include "camera.h"
#include "manager.h"
#include "texture.h"
#include "gameState.h"
#include "window.h"
#include "settings.h"

int main() {
	auto window = initializeWindow();
	glClearColor(1.f, 0.75f, 0.79f, 1);

	//Initialising ImGUI
	//TODO: Move this to windows.cpp
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	//	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	//Creating the sound engine
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

	//Initialising some singletons
	Loader loaderSingleton;
	Camera cameraMan;
	Settings settings;

	//Loading the settings from file
	if (!settings.initialise("./res/options.txt")) {
		std::cout << "Failed to load settings file!";
	}
	float renderDistance = settings.getElement("renderDistance");
	float musicVolume = settings.getElement("musicVolume");
	float sfxVolume = settings.getElement("sfxVolume");

	//Compiling the shader
	auto shader = makeShader("./res/fragment.shader", "./res/vertex.shader");
	glUseProgram(shader);

	//Creating storages for entities
	std::vector<Entity> radiators;
	std::vector<Entity> mexicans;
	std::vector<Entity> chilis;
	std::vector<Entity> tiles;

	//Some gameflow variables
	int level = 1;
	glm::vec3 spawnPosition;
	double xpos, ypos;
	double before = 0;
	double runTime = 0;
	double immunityTime = 0;
	int score = 0;
	int lives = 3;
	bool exit = false;
	bool firstFrame = true;
	bool immunity = false;

	//Some gameflow constants
	int const MAX_LEVEL = 3;
	double const IMMUNITY_BUFF_TIME = 3.0;

	//Initial game state: menu
	gameState state = mainMenu;

	//Loading music and sound effects
	irrklang::ISound* music = soundEngine->play2D("./res/sounds/music1.wav", GL_TRUE, GL_FALSE, GL_TRUE);
	music->setVolume(musicVolume);
	irrklang::ISoundSource* crunch = soundEngine->addSoundSourceFromFile("./res/sounds/crunch.mp3");
	crunch->setDefaultVolume(sfxVolume);
	irrklang::ISoundSource* busted = soundEngine->addSoundSourceFromFile("./res/sounds/guitar.wav");
	busted->setDefaultVolume(sfxVolume);

	irrklang::ISoundSource* gameOver = soundEngine->addSoundSourceFromFile("./res/sounds/gameover.wav");
	gameOver->setDefaultVolume(sfxVolume);

	//OpenGL window loop
	while (exit == 0 && !glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();

		//Main menu loop
		while (state == mainMenu) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (level > MAX_LEVEL) {
				level = 1;
			}

			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			bool startGame;
			bool openSettings;
			bool exitGame;

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			// Display Main Menu with start the game button
			ImGui::Begin("Main menu", NULL, window_flags);
			ImGui::SetWindowFontScale(2.f);
			ImGui::Text("Mexican Watermelon Haze");
			startGame = ImGui::Button("Start game");
			ImGui::Text("Level: %d", level);
			openSettings = ImGui::Button("Settings");
			exitGame = ImGui::Button("Exit");

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (startGame) {
				state = initialise;
			}

			while (openSettings){
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				int tempRenderDistance = int(renderDistance);

				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Settings", NULL, window_flags);
				ImGui::SetWindowFontScale(2.f);

				ImGui::SliderFloat("Music", &musicVolume, 0.0f, 1.0f);
				ImGui::SliderFloat("Sounds", &sfxVolume, 0.0f, 1.0f);
				ImGui::SliderInt("Render distance", &tempRenderDistance, 0.0f, 100.0f);
				bool save = ImGui::Button("Save");
				bool close = ImGui::Button("Back");


				ImGui::End();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				renderDistance = float(tempRenderDistance);

				if (close) {
					renderDistance = settings.getElement("renderDistance");
					musicVolume = settings.getElement("musicVolume");
					sfxVolume = settings.getElement("sfxVolume");
					openSettings = false;
				}

				if (save){

					openSettings = false;
					settings.setElement("renderDistance", renderDistance);
					settings.setElement("musicVolume", musicVolume);
					settings.setElement("sfxVolume", sfxVolume);


					music->setVolume(musicVolume);
					crunch->setDefaultVolume(sfxVolume);
					busted->setDefaultVolume(sfxVolume);
					gameOver->setDefaultVolume(sfxVolume);


				}

				glfwPollEvents();
				glfwSwapBuffers(window);

			}

			if (exitGame) {
				exit = true;
				break;
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		//Initialising
		if (state == initialise) {
			//I'm not sure if this does anything
			ImGui::EndFrame();

			//Loading the map
			std::string levelText = "./res/maps/map" + std::to_string(level) + ".txt";
			loadMap(levelText.c_str());

			spawnPosition = spawn(&loaderSingleton, &radiators, &mexicans, &chilis, &tiles);
			cameraMan.setCameraPosition(spawnPosition);

			score = 0;
			lives = 3;
			runTime = 0;
			immunityTime = 0;
			immunity = false;

			state = game;

			firstFrame = true;
		}

		//Game loop
		while (state == game) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			//Delta time stuff
			double now = glfwGetTime();
			double delta = now - before;
			if (firstFrame) {
				delta = 0;
				firstFrame = false;
			}
			before = now;

			//Buff manager
			if (immunity) {
				immunityTime += delta;
				if (immunityTime > IMMUNITY_BUFF_TIME) {
					immunity = false;
					immunityTime = 0.0;
				}
			}

			glUseProgram(shader);

			//Mouse controls
			glfwGetCursorPos(window, &xpos, &ypos);
			cameraMan.mouseCallback(xpos, ypos);
			cameraMan.calculateView(shader);

			//Checks in which direction the player can move
			//The vector only ever contains 4 objects
			//An array of bools would be better, but passing an array around might get messy
			std::vector<bool> walls;
			walls = checkIfWall(cameraMan.getCameraPosition(), cameraMan.getCameraFace());

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				if (!walls[1])
					cameraMan.moveCamera(1, delta);
			}

			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				if (!walls[3])
					cameraMan.moveCamera(3, delta);
			}

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				if (!walls[0])
					cameraMan.moveCamera(0, delta);
			}

			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				if (!walls[2])
					cameraMan.moveCamera(2, delta);
			}

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				state = pauseMenu;
			}

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				exit = true;
				break;
			}

			glfwPollEvents();

			glm::vec3 cameraPosition = cameraMan.getCameraPosition();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Managing the walls
			for (unsigned int i = 0; i < radiators.size(); i++) {
				glm::vec3 difference = cameraPosition - radiators[i].getPosition();

				if (length(difference) < renderDistance) {
					radiators[i].draw(shader);
				}
			}

			//Managing the floor tiles
			for (unsigned int i = 0; i < tiles.size(); i++) {
				glm::vec3 difference = cameraPosition - tiles[i].getPosition();
				glDisable(GL_CULL_FACE);

				if (length(difference) < renderDistance) {
					tiles[i].draw(shader);
				}
				
				glEnable(GL_CULL_FACE);
			}

			//Managing the pickup objects
			for (unsigned int i = 0; i < chilis.size(); i++) {
				glm::vec3 difference = cameraPosition - chilis[i].getPosition();

				if (eatPizza(cameraPosition, chilis[i])) {
					chilis.erase(chilis.begin() + i);
					score += 10;
					soundEngine->play2D(crunch);

					//Change level if no more chilis
					if (!chilis.size()) {
						level += 1;
						state = cleanup;
					}

					break;
				}

				//Just gently rotate the chilis in place
				chilis[i].rotate(1, 'y');
				glDisable(GL_CULL_FACE);

				if (length(difference) < renderDistance) {
					chilis[i].draw(shader);
				}

				glEnable(GL_CULL_FACE);
			}

			//Managing the enemies
			for (unsigned int i = 0; i < mexicans.size(); i++) {
				glm::vec3 difference = cameraPosition - mexicans[i].getPosition();
				// DEBUG A:
				if (pacmanBusted(cameraPosition, mexicans[i])) {
					if (!immunity) {
						cameraMan.setCameraPosition(spawnPosition);
						lives--;
						if (!lives) {
							soundEngine->play2D(gameOver);
							state = cleanup;
							level = 1;
							break;
						}
						immunity = true;
						immunityTime = 0.0;
						soundEngine->play2D(busted);
					}
				}
				moveGhost(&mexicans[i]);
				mexicans[i].moveAI(delta);

				if (length(difference) < renderDistance) {
					mexicans[i].draw(shader);
				}
			}

			runTime += delta;

			ImGui::SetNextWindowSize(ImVec2(200, 150));

			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			std::string scoreBuffer = "Score: " + std::to_string(score);
			std::string timeBuffer = "Time: " + std::to_string(int(runTime) / 60) + ":" + ((int(runTime) % 60) < 10 ? "0" : "") + std::to_string(int(runTime) % 60);
			std::string leftBuffer = "Left: " + std::to_string(chilis.size());
			std::string livesBuffer = "Lives: " + std::to_string(lives);
			std::string immunityBuffer = "Immunity: " + std::to_string(IMMUNITY_BUFF_TIME - immunityTime);

			//Specify what is supposed to display
			ImGui::Begin("Stats", NULL, window_flags);
			ImGui::SetWindowFontScale(2.f);
			ImGui::Text("%s", scoreBuffer.c_str());
			ImGui::Text("%s", timeBuffer.c_str());
			ImGui::Text("%s", leftBuffer.c_str());
			ImGui::Text("%s", livesBuffer.c_str());
			if (immunity) {
				ImGui::Text("%s", immunityBuffer.c_str());
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

		//Pause loop
		while (state == pauseMenu) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			bool continueGame;
			bool mainMenu;
			bool openSettings;
			bool exitGame;

			// Display Main Menu with start the game button
			ImGui::Begin("Pause", NULL, window_flags);
			ImGui::SetWindowFontScale(2.f);
			ImGui::Text("Game Paused");
			continueGame = ImGui::Button("Continue");
			mainMenu = ImGui::Button("Main menu");
			openSettings = ImGui::Button("Settings");
			exitGame = ImGui::Button("Exit");

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (continueGame || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				state = game;
			}

			if (mainMenu) {
				state = cleanup;
			}

			while (openSettings){
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				int tempRenderDistance = int(renderDistance);

				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Settings", NULL, window_flags);
				ImGui::SetWindowFontScale(2.f);

				ImGui::SliderFloat("Music", &musicVolume, 0.0f, 1.0f);
				ImGui::SliderFloat("Sounds", &sfxVolume, 0.0f, 1.0f);
				ImGui::SliderInt("Render distance", &tempRenderDistance, 0.0f, 100.0f);
				bool save = ImGui::Button("Save");
				bool close = ImGui::Button("Back");


				ImGui::End();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				renderDistance = float(tempRenderDistance);


				if (close) {
					renderDistance = settings.getElement("renderDistance");
					musicVolume = settings.getElement("musicVolume");
					sfxVolume = settings.getElement("sfxVolume");
					openSettings = false;
				}

				if (save){

					openSettings = false;
					settings.setElement("renderDistance", renderDistance);
					settings.setElement("musicVolume", musicVolume);
					settings.setElement("sfxVolume", sfxVolume);


					music->setVolume(musicVolume);
					crunch->setDefaultVolume(sfxVolume);
					busted->setDefaultVolume(sfxVolume);
					gameOver->setDefaultVolume(sfxVolume);


				}

				glfwPollEvents();
				glfwSwapBuffers(window);

			}


			if (exitGame) {
				exit = true;
				break;
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		//Clearing up
		if (state == cleanup) {
			for (unsigned int i = 0; i < radiators.size(); i++) {
				radiators[i].imminentOblivion();
			}
			for (unsigned int i = 0; i < mexicans.size(); i++) {
				mexicans[i].imminentOblivion();
			}
			for (unsigned int i = 0; i < chilis.size(); i++) {
				chilis[i].imminentOblivion();
			}
			for (unsigned int i = 0; i < tiles.size(); i++) {
				tiles[i].imminentOblivion();
			}
			radiators.clear();
			mexicans.clear();
			chilis.clear();
			tiles.clear();

			state = mainMenu;
		}
	}

	glUseProgram(0u);
	glDeleteProgram(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
