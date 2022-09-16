#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "vertex.hh"

class Object {
public:
    glm::vec3 origin;
    virtual std::pair<std::vector<Vertex>, std::vector<uint16_t>> loadVertices() const = 0;
};

class Cube : public Object {
public:
    float length;
    Cube(glm::vec3 o, float l) {
        origin = o;
        length = l;
    }
    std::pair<std::vector<Vertex>, std::vector<uint16_t>> loadVertices() const;
};