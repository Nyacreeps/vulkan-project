#include "app.hh"

#include <string>
#include <set>
#include <algorithm>

extern int KEYSTATES[512];
extern bool KEY_INPUT_DETECTED;

void VulkanApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
}

void VulkanApplication::initVulkan() {
    createInstance("potato", "No Engine");
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createAllocator();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createDepthResources();
    createFramebuffers();
    createCommandPools();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
    registerInputFunctions();
}

void VulkanApplication::mainLoop() {
    
    double lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = (float)(currentFrame - lastFrame);
        if (deltaTime < (1.0f / 30.0f)) continue;
        glfwPollEvents();
        inputCamera(deltaTime);
        drawFrame();
        lastFrame = currentFrame;
    }
    vkDeviceWaitIdle(device);
}