#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <iostream>

#include "vertex.hh"
#include "camera.hh"
#include "object.hh"

class Light {
    glm::vec3 position;
};

class Scene {
public:
    Scene(std::shared_ptr<Camera> cam) : pCamera(cam){};
    uint32_t indexBufferSize = 0;
    std::shared_ptr<std::pair<std::vector<Vertex>, std::vector<uint16_t>>> pVertexData;
    std::pair<std::vector<Vertex>, std::vector<uint16_t>> loadAllVertices(void);
    std::shared_ptr<Camera> pCamera;
    std::vector<std::shared_ptr<Light>> pLights;
    std::vector<std::shared_ptr<Object>> pObjects;
};