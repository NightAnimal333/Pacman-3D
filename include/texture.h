#pragma once

#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <string>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>

class Texture {

private:

	GLuint textureID;
	//Used by destructor
	bool trueDestroy = 0;

public:

	Texture() = default;
	~Texture();
	//Loads the texture from file
	void load(const std::string& filepath);

	//Call to destory an object completely and wipe it from OpenGL
	void imminentOblivion();

	GLuint getID();

};

#endif
