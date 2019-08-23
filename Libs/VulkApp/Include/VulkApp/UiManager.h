#pragma once

#include "GpuBuffer.h"
#include "Texture.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace render
{
    class VulkanContext;
}

class UiManager
{
public:
    UiManager(render::VulkanContext* vkContext);
    ~UiManager();

    void Initialize(float width, float height, float fbWidth, float fbHeight, VkRenderPass renderPass);
    void CleanupSwapChainResources();

    void BuildCommandBuffer(VkCommandBuffer cmdBuffer, float deltaTime);
    void Render();

    bool UpdateMousePos(bool windowFocus, uint8_t mouseButtons, double& x, double& y);

protected:
    void CreateGraphicsPipeline(VkRenderPass renderPass);
    void UpdateBuffers();

private:
    render::VulkanContext* m_vkContext;
    render::VulkanTexture  m_fontTexture;
    VkDescriptorPool       m_descriptorPool;
    VkDescriptorSetLayout  m_descriptorSetLayout;
    VkDescriptorSet        m_descriptorSet;
    VkPipelineCache        m_pipelineCache;
    VkPipelineLayout       m_pipelineLayout;
    VkPipeline             m_pipeline;
    render::GpuBuffer      m_vertexBuffer;
    render::GpuBuffer      m_indexBuffer;
    uint32_t               m_vertexCount;
    uint32_t               m_indexCount;

    struct PushConstBlock
    {
        glm::vec2 scale;
        glm::vec2 translate;
    };
    PushConstBlock m_pushConstBlock;
};
