#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "vertex.hh"

class Camera {
    
};

class Light {
    glm::vec3 position;
};

class Object {
    glm::vec3 origin;
    virtual std::vector<Vertex> getVertices() = 0;
};

class Scene {
    Camera camera;
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Object>> objects;
};