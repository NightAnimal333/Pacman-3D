#include <iostream>

#include "texture.h"

Texture::~Texture()
{
	if (!trueDestroy) {
		return;
	}
	glDeleteTextures(1, &this->textureID);
}

void Texture::load(const std::string& filepath) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, components;
	unsigned char* image = stbi_load(filepath.c_str(), &width, &height, &components, STBI_default);

	if (!image) {
		std::cout << "Failed to load texture: " << filepath.c_str() << '\n';
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	GLenum format;
	if (components == 1)
		format = GL_RED;
	else if (components == 3)
		format = GL_RGB;
	else if (components == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);

	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint Texture::getID() {
	return this->textureID;
}

//Set the object to be wiped from GPU on next destructor call
void Texture::imminentOblivion() {
	trueDestroy = 1;
}
