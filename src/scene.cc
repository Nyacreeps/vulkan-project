#include "scene.hh"

#include <iostream>

std::pair<std::vector<Vertex>, std::vector<uint16_t>> Scene::loadAllVertices() {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    size_t numIndex = 0;
    for (auto &pObject : pObjects) {
        auto vertIndexPair = pObject->loadVertices();
        vertices.reserve(vertices.size() + vertIndexPair.first.size());
        indices.reserve(indices.size() + vertIndexPair.second.size());

        vertices.insert(vertices.end(), vertIndexPair.first.begin(), vertIndexPair.first.end());
        for (size_t i = 0; i < vertIndexPair.second.size(); i++) {
            vertIndexPair.second[i] += numIndex;
        }
        indices.insert(indices.end(), vertIndexPair.second.begin(), vertIndexPair.second.end());
        numIndex += vertIndexPair.first.size();
    }
    indexBufferSize = indices.size();
    pVertexData = std::make_shared<std::pair<std::vector<Vertex>, std::vector<uint16_t>>>(vertices, indices);
    return *pVertexData;
}