#include "app.hh"

#include <string>
#include <set>
#include <algorithm>

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
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPools();
    createAllocator();
    createVertexBuffer();
    createIndexBuffer();
    createCommandBuffers();
    createSyncObjects();

    /* uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    VkPhysicalDeviceProperties physicalDeviceProps{};

    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProps);
    std::cout << "physical device: " << physicalDeviceProps.deviceName << '\n'; */
}

void VulkanApplication::mainLoop() {
    while (!glfwWindowShouldClose(appWindow)) {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(device);
}