#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include "app.hh"
#include "scene.hh"

int main() {
    Scene scene(Camera({-6.0f, 0.0f, 0.0f}, {0.0f, 0.0f ,0.0f}, {0.0f, 0.0f, 1.0f}, M_PI_2, 0.1f, 10.0f));
    auto app = VulkanApplication();
    app.setScene(scene);
    app.run();

    return 0;
}