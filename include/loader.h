#pragma once

#ifndef __LOADER_H
#define __LOADER_H

#include "entity.h"
#include "texture.h"

#include <iostream>
#include <string>

class Loader{

private:

//Much empty

public:

Loader() = default;

//Loads an object from file
Entity load(std::string objectPath, Texture tempTexture);

//Loads a 2D sprite
Entity loadSprite(Texture tempTexture);


};

#endif
