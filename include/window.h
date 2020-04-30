#pragma once

#ifndef __WINDOW_H
#define __WINDOW_H

#include <string>
#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "gfx.h"

//Starts the winodw and initializes all libraries and enables specific openGL settings
GLFWwindow* initializeWindow();

//Reads a shader file
std::string readFile(char* source);

//Create a shader
GLuint makeShader(const char* fragSource, const char* vertSource);

#endif
