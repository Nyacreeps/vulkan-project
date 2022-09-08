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
#include "scene.hh"

class VulkanApplication {
public:
    VulkanApplication(){};
    ~VulkanApplication() {
        cleanupSwapChain();

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vmaFreeMemory(allocator, vertexBufferAllocation);

        vkDestroyBuffer(device, indexBuffer, nullptr);
        vmaFreeMemory(allocator, indexBufferAllocation);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vmaFreeMemory(allocator, uniformBuffersAllocation[i]);
        }

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

        glfwDestroyWindow(window);

        glfwTerminate();
    };
    void run();

private:
    // window setup
    GLFWwindow* window = nullptr;
    uint windowWidth = 800;
    uint windowHeight = 600;
    void initWindow();

    VkSurfaceKHR surface;
    void createSurface();



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



    // swap chain and pipeline setup
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

    VkDescriptorSetLayout descriptorSetLayout;
    void createDescriptorSetLayout();

    VkPipeline graphicsPipeline;
    void createGraphicsPipeline();

    std::vector<VkFramebuffer> swapChainFramebuffers;
    void createFramebuffers();

    VkCommandPool graphicsCommandPool;
    VkCommandPool transferCommandPool;
    bool poolsSameHandle = false;
    void createCommandPools();



    // depth map setup
    VkImage depthImage;
    VmaAllocation depthImageAllocation;
    VkImageView depthImageView;
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                     VmaAllocation& imageAllocation);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createDepthResources();



    // Buffers allocation and update
    const Cube cube = Cube({0.0f, 0.0f, 0.0f}, 1.0f);
    VmaAllocator allocator;
    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;
    VkBuffer indexBuffer;
    VmaAllocation indexBufferAllocation;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBuffersAllocation;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags flags,
                      VkBuffer& buffer, VmaAllocation& bufferAllocation);
    void createAllocator();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void updateUniformBuffers();

    VkDescriptorPool descriptorPool;
    void createDescriptorPool();
    std::vector<VkDescriptorSet> descriptorSets;
    void createDescriptorSets();

    std::vector<VkCommandBuffer> commandBuffers;
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    void createSyncObjects();

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

public:
    bool framebufferResized = false;
private:
    Scene* scene = nullptr;

public:
    void setScene(Scene& scene);

private:
    void registerInputFunctions();
    void inputCamera(float deltaTime);
    void drawFrame();

    void cleanupSwapChain();
    void recreateSwapChain();

    void initVulkan();
    void mainLoop();
};