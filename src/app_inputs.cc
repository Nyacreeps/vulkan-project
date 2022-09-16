#include "app.hh"

#include <iostream>
#include <cstring>

int KEYSTATES[512] = {0};
int KEYPRESSES[512] = {0};
bool KEY_INPUT_DETECTED = false;
extern int VERTEX_UPDATE;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        KEYSTATES[key] = 1;
        if (KEYPRESSES[key] == 0)
            KEYPRESSES[key] = 1;
    }
    if (action == GLFW_RELEASE) {
        KEYSTATES[key] = 0;
        KEYPRESSES[key] = 0;
    }
}

void VulkanApplication::setScene(std::shared_ptr<Scene> pScene) {
    this->pScene = pScene;
}

void VulkanApplication::registerInputFunctions() {
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    // prevents missing key events but often cause them to be registered 2 times
    // should not be a problem most of the time
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void VulkanApplication::inputCamera(float deltaTime) {
    if (KEYSTATES[GLFW_KEY_W] == 1) {
        if (KEYSTATES[GLFW_KEY_A] == 1) {
            pScene->pCamera->move(CAM_MOVE_LEFT, deltaTime, 1.0f);
        }
        pScene->pCamera->move(CAM_MOVE_FRONT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_A] == 1) {
        pScene->pCamera->move(CAM_MOVE_LEFT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_S] == 1) {
        pScene->pCamera->move(CAM_MOVE_BACK, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_D] == 1) {
        pScene->pCamera->move(CAM_MOVE_RIGHT, deltaTime, 1.0f);
    }
    if (KEYSTATES[GLFW_KEY_C] == 1) {
        pScene->pCamera->move(CAM_MOVE_DOWN, deltaTime, 1.8f);
    }
    if (KEYSTATES[GLFW_KEY_SPACE] == 1) {
        pScene->pCamera->move(CAM_MOVE_UP, deltaTime, 1.8f);
    }
    if (KEYPRESSES[GLFW_KEY_R] == 1) {
        std::cout << "spawning cube\n";
        pScene->pObjects.push_back(std::move(std::make_shared<Cube>(Cube(pScene->pCamera->center, 1.0f))));
        KEYPRESSES[GLFW_KEY_R] = 2;
        VERTEX_UPDATE = 1;
    }
    double mousePos[2] = {0};
    glfwGetCursorPos(window, mousePos, mousePos + 1);
    pScene->pCamera->roll((windowWidth / 2) - mousePos[0], mousePos[1] - (windowHeight / 2), 0.5f);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
}