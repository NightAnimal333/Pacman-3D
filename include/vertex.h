#pragma once

#ifndef __VERTEX_H
#define __VERTEX_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vertex {

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;


  bool operator==(const Vertex& other) const;

};

#endif
