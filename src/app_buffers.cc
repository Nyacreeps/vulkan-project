#include "app.hh"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.hh"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

VkDeviceSize BUFFER_SIZE = UINT16_MAX;

uint32_t VulkanApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transferCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate command buffer for buffer copy");

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        throw std::runtime_error("failed to begin recording copy command buffer");

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to end recording copy command buffer");

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    if (vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        throw std::runtime_error("failed to submit copy command buffer to queue");
    vkQueueWaitIdle(transferQueue);

    vkFreeCommandBuffers(device, transferCommandPool, 1, &commandBuffer);
}

void VulkanApplication::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                     VmaAllocationCreateFlags flags, VkBuffer& buffer,
                                     VmaAllocation& bufferAllocation) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocationCreateInfo{};
    allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocationCreateInfo.flags = flags; // EUH

    vmaCreateBuffer(allocator, &bufferInfo, &allocationCreateInfo, &buffer, &bufferAllocation,
                    nullptr);
}

void VulkanApplication::createAllocator() {
    VmaAllocatorCreateInfo allocatorCreateInfo{};
    allocatorCreateInfo.physicalDevice = physicalDevice;
    allocatorCreateInfo.device = device;
    allocatorCreateInfo.instance = instance;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;

    vmaCreateAllocator(&allocatorCreateInfo, &allocator);
}

void VulkanApplication::createStagingBuffer() {
    createBuffer(BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT, stagingBuffer,
                 stagingBufferAllocation);
}

void VulkanApplication::createVertexBuffer() {
    createBuffer(BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 0, vertexBuffer, vertexBufferAllocation);
}

void VulkanApplication::createIndexBuffer() {
    createBuffer(BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 0, indexBuffer, indexBufferAllocation);
}

void VulkanApplication::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT, uniformBuffers[i],
                     uniformBuffersAllocation[i]);
    }

    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = pScene->pCamera->getViewMatrix();
    ubo.proj =
        pScene->pCamera->getProjectionMatrix(swapChainExtent.width / (float)swapChainExtent.height);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        void* data;
        vmaMapMemory(allocator, uniformBuffersAllocation[i], &data);
        memcpy(data, &ubo, sizeof(ubo));
        vmaUnmapMemory(allocator, uniformBuffersAllocation[i]);
    }
}

void VulkanApplication::updateUniformBuffer(uint32_t currentImage) {
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = pScene->pCamera->getViewMatrix();
    ubo.proj =
        pScene->pCamera->getProjectionMatrix(swapChainExtent.width / (float)swapChainExtent.height);

    void* data;
    vmaMapMemory(allocator, uniformBuffersAllocation[currentImage], &data);
    memcpy(data, &ubo, sizeof(ubo));
    vmaUnmapMemory(allocator, uniformBuffersAllocation[currentImage]);
}

void VulkanApplication::updateVertexBuffer() {
    auto vert = pScene->loadAllVertices().first;
    VkDeviceSize bufferSize = sizeof(vert[0]) * vert.size();

    void* data;
    if (vmaMapMemory(allocator, stagingBufferAllocation, &data) != VK_SUCCESS)
        throw std::runtime_error("failed to map allocation memory");
    memcpy(data, vert.data(), (size_t)bufferSize);
    vmaUnmapMemory(allocator, stagingBufferAllocation);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
}

void VulkanApplication::updateIndexBuffer() {
    auto index = pScene->pVertexData->second;
    VkDeviceSize bufferSize = sizeof(index[0]) * index.size();

    void* data;
    if (vmaMapMemory(allocator, stagingBufferAllocation, &data) != VK_SUCCESS)
        throw std::runtime_error("failed to map allocation memory");
    memcpy(data, index.data(), (size_t)bufferSize);
    vmaUnmapMemory(allocator, stagingBufferAllocation);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);
}