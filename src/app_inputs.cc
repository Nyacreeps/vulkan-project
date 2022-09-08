#include "app.hh"

#include <iostream>
#include <cstring>

int KEYSTATES[512] = {0};
int KEYPRESSES[512] = {0};
bool KEY_INPUT_DETECTED = false;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::memset(KEYPRESSES, 0, 512);
    if (action == GLFW_PRESS) {
        KEYSTATES[key] = 1;
    }
    if (action == GLFW_RELEASE) {
        KEYSTATES[key] = 0;
    }
    if (action == GLFW_REPEAT) {
        KEYPRESSES[key] = 1;
    }
}

void VulkanApplication::setScene(Scene& scene) {
    this->scene = &scene;
}

void VulkanApplication::registerInputFunctions() {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    // prevents missing key events but often cause them to be registered 2 times
    // should not be a problem most of the time
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void VulkanApplication::inputCamera(float deltaTime) {
    if (KEYSTATES[GLFW_KEY_W] == 1) {
        if (KEYSTATES[GLFW_KEY_A] == 1) {
            scene->camera.move(CAM_MOVE_LEFT, deltaTime, 1.0f);
        }
        scene->camera.move(CAM_MOVE_FRONT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_A] == 1) {
        scene->camera.move(CAM_MOVE_LEFT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_S] == 1) {
        scene->camera.move(CAM_MOVE_BACK, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_D] == 1) {
        scene->camera.move(CAM_MOVE_RIGHT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_C] == 1) {
        scene->camera.move(CAM_MOVE_DOWN, deltaTime, 1.8f);
    }
    if (KEYSTATES[GLFW_KEY_SPACE] == 1) {
        scene->camera.move(CAM_MOVE_UP, deltaTime, 1.8f);
    }
    double mousePos[2] = {0};
    glfwGetCursorPos(window, mousePos, mousePos + 1);
    scene->camera.roll((windowWidth / 2) - mousePos[0], mousePos[1] - (windowHeight / 2), 0.5f);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
}