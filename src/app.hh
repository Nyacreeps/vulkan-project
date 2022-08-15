#pragma once

#include "vk_mem_alloc.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

#include "vertex.hh"

class VulkanApplication {
public:
    VulkanApplication(){};
    VulkanApplication(const VulkanApplication& app) {
        this->appWindow = app.appWindow;
        this->windowWidth = app.windowWidth;
        this->windowHeight = app.windowHeight;
    };
    VulkanApplication& operator=(const VulkanApplication& app) {
        if (this == &app) return *this;
        glfwDestroyWindow(appWindow);
        appWindow = app.appWindow;
        windowWidth = app.windowWidth;
        windowHeight = app.windowHeight;
        return *this;
    };
    ~VulkanApplication() {
        cleanupSwapChain();

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vmaFreeMemory(allocator, vertexBufferAllocation);

        vmaDestroyAllocator(allocator);

        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

        vkDestroyRenderPass(device, renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(device, graphicsCommandPool, nullptr);
        if (!poolsSameHandle) vkDestroyCommandPool(device, transferCommandPool, nullptr);

        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(appWindow);

        glfwTerminate();
    };
    void run();

private:
    // window setup
    GLFWwindow* appWindow = nullptr;
    uint windowWidth = 800;
    uint windowHeight = 600;
    void initWindow();

    // instance and validation layers setup
    VkInstance instance = nullptr; // link between the application and the vulkan library
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void createInstance(const std::string&, const std::string&);

    // debug log setup
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT*,
                                                 const VkAllocationCallbacks*,
                                                 VkDebugUtilsMessengerEXT*);
    static void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT,
                                              const VkAllocationCallbacks*);
    VkDebugUtilsMessengerEXT debugMessenger;
    void setupDebugMessenger();

    VkSurfaceKHR surface;
    void createSurface();

    // physical device setup
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> transferFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value() &&
                   transferFamily.has_value();
        }
    };
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();

    // logical device and queue setup
    VkDevice device;
    VkQueue presentQueue;
    VkQueue graphicsQueue;
    VkQueue transferQueue;
    void createLogicalDevice();

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();

    std::vector<VkImageView> swapChainImageViews;
    void createImageViews();

    VkRenderPass renderPass;
    void createRenderPass();

    VkPipelineLayout pipelineLayout;
    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkPipeline graphicsPipeline;
    void createGraphicsPipeline();

    std::vector<VkFramebuffer> swapChainFramebuffers;
    void createFramebuffers();

    VkCommandPool graphicsCommandPool;
    VkCommandPool transferCommandPool;
    bool poolsSameHandle = false;
    void createCommandPools();

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}}, {{0.5f, -0.5f}}, {{0.5f, 0.5f}}};
    VmaAllocator allocator;
    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VmaAllocation& bufferAllocation);
    void createAllocator();
    void createVertexBuffer();

    std::vector<VkCommandBuffer> commandBuffers;
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    void createSyncObjects();

    void initVulkan();

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

public:
    bool framebufferResized = false;

private:
    void drawFrame();

    void cleanupSwapChain();
    void recreateSwapChain();

    void mainLoop();
};