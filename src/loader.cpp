#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <stdexcept>
#include <unordered_map>

#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

#include "loader.h"
#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "vertex.h"



namespace std {
  template<> struct hash<Vertex> {
      size_t operator()(Vertex const& vertex) const {
          return ((hash<glm::vec3>()(vertex.position) ^
                 (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                 (hash<glm::vec2>()(vertex.texture) << 1);
      }
  };
}

Entity Loader::load(std::string objectPath, Texture tempTexture){


  tinyobj::attrib_t attributes{};
  std::vector<tinyobj::shape_t> shapes{};
  std::vector<tinyobj::material_t> materials{};
  std::string warning{}, error{};

  if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, objectPath.c_str()))
  {
    std::cout << "Could not load model: W(%s) E(%s)", warning.c_str(), error.c_str();
  }

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  Entity entity;

  std::unordered_map<Vertex, uint32_t> uniqueVertices = {};


  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
        Vertex vertex = {};

        vertex.position = {
        attributes.vertices[3 * index.vertex_index + 0],
        attributes.vertices[3 * index.vertex_index + 1],
        attributes.vertices[3 * index.vertex_index + 2]
        };

        vertex.normal = {
        attributes.vertices[3 * index.normal_index + 0],
        attributes.vertices[3 * index.normal_index + 1],
        attributes.vertices[3 * index.normal_index + 2]
        };

        vertex.texture = {
        attributes.texcoords[2 * index.texcoord_index + 0],
        attributes.texcoords[2 * index.texcoord_index + 1]
        };


        if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
        }

        indices.push_back(uniqueVertices[vertex]);

      }
    }

    entity.addUniqueVertices(vertices);

    std::vector<GLuint> threeIndices;
    std::vector<Mesh> meshes;
    for (unsigned int i = 0; i < indices.size(); i++){

      threeIndices.push_back(indices[i]);

      if (threeIndices.size() == 3){

        Mesh tempMesh(threeIndices);
        meshes.push_back(tempMesh);
        threeIndices.clear();

      }

    }


    for (unsigned int i = 0; i < meshes.size(); i++){

        meshes[i].setTexture(tempTexture);
      entity.addMesh(meshes[i]);

    }

    return entity;
}

Entity Loader::loadSprite(Texture tempTexture){

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices1 = {0, 1, 2};
  std::vector<GLuint> indices2 = {0, 3, 2};

  Entity entity;

  Vertex tempVertex = {};

  for (int i = 0; i < 4; i++){

    vertices.push_back(tempVertex);

  }

  vertices[0].position = glm::vec3(0.f, 0.f, 0.f);
  vertices[1].position = glm::vec3(0.f, 1.f, 0.f);
  vertices[2].position = glm::vec3(1.f, 1.f, 0.f);
  vertices[3].position = glm::vec3(1.f, 0.f, 0.f);

  for (int i = 0; i < 4; i++){

    vertices[i].normal = glm::vec3(0.f, 0.f, 0.f);

  }

  vertices[0].texture = glm::vec2(0.f, 0.f);
  vertices[1].texture = glm::vec2(0.f, 1.f);
  vertices[2].texture = glm::vec2(1.f, 1.f);
  vertices[3].texture = glm::vec2(1.f, 0.f);

  entity.addUniqueVertices(vertices);

  Mesh tempMesh1(indices1);
  Mesh tempMesh2(indices2);

  tempMesh1.setTexture(tempTexture);
  tempMesh2.setTexture(tempTexture);

  entity.addMesh(tempMesh1);
  entity.addMesh(tempMesh2);


  return entity;


}
