#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include "app.hh"

int main() {
    auto app = VulkanApplication();
    app.run();

    return 0;
}