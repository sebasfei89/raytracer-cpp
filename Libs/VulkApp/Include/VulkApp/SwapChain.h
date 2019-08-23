#pragma once

#include "Image.h"
#include "VulkanUtils.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace render
{

class VulkanSwapChain
{
public:
    VulkanSwapChain();

    void Initialize(VulkanContext const& vkContext, VkSurfaceKHR surface, uint32_t width, uint32_t height);
    void Finalize(VulkanContext const& vkContext);

    void CreateResources(VulkanContext const& vkContext, VkRenderPass renderPass);
    void CleanupResources(VulkanContext const& vkContext);

    VkExtent2D Extent() const { return m_extent; }
    VkFormat ImageFormat() const { return m_swapChainImageFormat; }

    // Framebuffer
    size_t FramebuffersCount() const { return m_swapChainFramebuffers.size(); }
    VkFramebuffer Framebuffer(size_t idx) { return m_swapChainFramebuffers[idx]; }
    VkImageView ImageView(size_t idx) { return m_swapChainImages[idx].ImageView(); }

    bool AcquireNextImage(VulkanContext const& vkContext, VkSemaphore semaphore, uint32_t* imageIndex);
    size_t ImageCount() const { return m_swapChainImages.size(); }
    VkSwapchainKHR Get() const { return m_swapChain; }

    VkFormat FindDepthFormat(VkPhysicalDevice vkPhysicalDevice) const;

protected:
    void CreateColorResources(VulkanContext const& vkContext);
    void CreateDepthResources(VulkanContext const& vkContext);
    void CreateFramebuffers(VulkanContext const& vkContext, VkRenderPass renderPass);

private:
    VkSwapchainKHR             m_swapChain;
    std::vector<VulkanImage>   m_swapChainImages;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkFormat                   m_swapChainImageFormat;
    VkExtent2D                 m_extent;
    VulkanImage                m_colorImage;
    VulkanImage                m_depthImage;
};

}
