#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include "app.hh"
#include "scene.hh"

int main() {
    std::shared_ptr<Scene> pScene = std::make_shared<Scene>(std::make_shared<Camera>(
        Camera({-6.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, M_PI_2, 0.1f, 10.0f)));
    pScene->pObjects.push_back(std::move(std::make_shared<Cube>(Cube({0.0f, 0.0f, 0.0f}, 1.0f))));
    auto app = VulkanApplication();
    app.setScene(pScene);
    app.run();

    return 0;
}