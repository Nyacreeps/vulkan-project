#include "app.hh"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void VulkanApplication::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    appWindow = glfwCreateWindow(windowWidth, windowHeight, "", nullptr, nullptr);
    glfwSetWindowUserPointer(appWindow, this);
    glfwSetFramebufferSizeCallback(appWindow, framebufferResizeCallback);
}