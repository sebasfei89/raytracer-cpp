#pragma once

#include "vulkapp_export.h"

#include "GpuBuffer.h"
#include "SwapChain.h"
#include "Texture.h"
#include "VulkanUtils.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <optional>
#include <vector>

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;

    bool operator==(Vertex const& other) const
    {
        return pos == other.pos && texCoord == other.texCoord;
    }

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

class UiManager;

class VulkanApplication
{
public:
    VULKAPP_EXPORT VulkanApplication();

    VULKAPP_EXPORT bool Init();
    VULKAPP_EXPORT void Run();

    VULKAPP_EXPORT void UpdateTexture(unsigned char* pixels, uint32_t width, uint32_t height);

    void OnFrameBufferResized() { m_framebufferResized = true; }

protected:
    void MainLoop();
    void DrawFrame(size_t currentFrame, float deltaTime);
    void UpdateUniformBuffer(uint32_t imageIndex);

    void CleanUp();

    void InitWindow();
    void InitVulkan();
    void LoadModel();

    // Vulkan specifics
    void SetupDebugMessenger();
    void RecreateSwapChain();
    void CleanupSwapChain();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateCommandBuffers();
    void BuildCommandBuffer(uint32_t imageIdx, float deltaTime);
    void CreateSyncObjects();
    void CreateDescriptorSets();
    void UpdateDescriptorSets();

private:
    GLFWwindow*                    m_window;
    render::VulkanContext          m_vkContext;
    VkDebugUtilsMessengerEXT       m_debugMessenger;
    VkSurfaceKHR                   m_surface;
    render::VulkanSwapChain        m_swapChain;
    VkRenderPass                   m_renderPass;
    VkDescriptorSetLayout          m_descriptorSetLayout;
    VkPipelineLayout               m_pipelineLayout;
    VkPipeline                     m_graphicsPipeline;
    std::vector<VkCommandBuffer>   m_commandBuffers;
    std::vector<VkSemaphore>       m_imageAvailableSemaphores;
    std::vector<VkSemaphore>       m_renderFinishedSemaphores;
    std::vector<VkFence>           m_inFlightFences;
    bool                           m_framebufferResized;
    std::vector<render::GpuBuffer> m_uniformBuffers;
    VkDescriptorPool               m_descriptorPool;
    std::vector<VkDescriptorSet>   m_descriptorSets;

    // Texture & sampler resources
    render::VulkanTexture        m_texture;

    // Model
    std::vector<Vertex>          m_vertices;
    std::vector<uint32_t>        m_indices;
    render::GpuBuffer            m_vertexBuffer;
    render::GpuBuffer            m_indexBuffer;

    UiManager*                   m_uiManager;
};
