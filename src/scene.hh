#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <iostream>

#include "vertex.hh"
#include "camera.hh"

class Light {
    glm::vec3 position;
};

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
    std::pair<std::vector<Vertex>, std::vector<uint16_t>> loadVertices() const {
        // stupid
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        for (int i = -1; i < 2; i += 2) {
            for (int j = -1; j < 2; j += 2) {
                for (int k = -1; k < 2; k += 2) {
                    vertices.push_back({{origin.x + i * length / 2, origin.y + j * length / 2,
                                         origin.z + k * length / 2},
                                        {1.0f, 0.0f, 0.0f},
                                        {0.0f, 0.0f}});
                }
            }
        }
        indices.resize(36);
        indices.assign({1, 7, 3, 1, 5, 7, 5, 6, 7, 5, 4, 6, 4, 0, 6, 4, 2, 6,
                        0, 1, 3, 0, 3, 2, 0, 5, 1, 0, 4, 5, 3, 6, 2, 3, 7, 6});
        return std::pair<std::vector<Vertex>, std::vector<uint16_t>>(vertices, indices);
    };
};

class Scene {
public:
    Scene(Camera cam) : camera(cam){};
    Camera camera;
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Object>> objects;
};