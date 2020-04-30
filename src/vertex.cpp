#include "vertex.h"

bool Vertex::operator==(const Vertex& other) const {
    return position == other.position && texture == other.texture && normal == other.normal;
}
