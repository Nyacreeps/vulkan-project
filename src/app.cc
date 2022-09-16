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
    std::cout << "SETUP : creating instance\n";
    createInstance("potato", "No Engine");
    std::cout << "SETUP : setting up debug\n";
    setupDebugMessenger();
    std::cout << "SETUP : creating surface\n";
    createSurface();
    std::cout << "SETUP : setting up physical device\n";
    pickPhysicalDevice();
    std::cout << "SETUP : creating logical device\n";
    createLogicalDevice();
    std::cout << "SETUP : creating allocator\n";
    createAllocator();
    std::cout << "SETUP : creating swapchain\n";
    createSwapChain();
    std::cout << "SETUP : setting up image views\n";
    createImageViews();
    std::cout << "SETUP : setting up render pass\n";
    createRenderPass();
    std::cout << "SETUP : creating descriptor layout\n";
    createDescriptorSetLayout();
    std::cout << "SETUP : creating graphics pipeline\n";
    createGraphicsPipeline();
    std::cout << "SETUP : setting up depth buffering\n";
    createDepthResources();
    std::cout << "SETUP : creating framebuffers\n";
    createFramebuffers();
    std::cout << "SETUP : creating command pools\n";
    createCommandPools();
    std::cout << "SETUP : creating staging buffer\n";
    createStagingBuffer();
    std::cout << "SETUP : creating vertex buffer\n";
    createVertexBuffer();
    std::cout << "SETUP : updating vertex buffer\n";
    updateVertexBuffer();
    std::cout << "SETUP : creating index buffer\n";
    createIndexBuffer();
    updateIndexBuffer();
    std::cout << "SETUP : creating uniform buffers\n";
    createUniformBuffers();
    std::cout << "SETUP : creating descriptor pool\n";
    createDescriptorPool();
    std::cout << "SETUP : creating descriptor sets\n";
    createDescriptorSets();
    std::cout << "SETUP : creating command buffers\n";
    createCommandBuffers();
    std::cout << "SETUP : creating sync objects\n";
    createSyncObjects();
    std::cout << "SETUP : registering input functions\n";
    registerInputFunctions();
}

void VulkanApplication::mainLoop() {
    std::cout << "SETUP : starting main loop\n";
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