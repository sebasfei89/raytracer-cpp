#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <string>
#include <vector>
#include <stdexcept>

namespace render
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR>   m_presentModes;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> m_graphicsFamily;
        std::optional<uint32_t> m_presentFamily;

        bool IsComplete() const { return m_graphicsFamily.has_value() && m_presentFamily.has_value(); }
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    class VulkanContext
    {
    public:
        static void VkCheck(VkResult result, std::string const& errorMsg) { if (result != VK_SUCCESS) throw std::runtime_error(errorMsg); }

        VulkanContext();

        void CreateInstance(bool enableValidationLayers);
        void PickPhysicalDevice(VkSurfaceKHR surface);
        void CreateLogicalDevice(bool validationLayersEnabled, VkSurfaceKHR surface);
        void CreateCommandPool(VkSurfaceKHR surface);

        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) const;
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) const;
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;
        void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) const;

        VkCommandBuffer BeginSingleTimeCommands() const;
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

        void WaitDeviceIdle() const { vkDeviceWaitIdle(m_device); }

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        VkInstance       Instance() const { return m_instance; }
        VkPhysicalDevice PhysicalDevice() const { return m_physicalDevice; }
        VkDevice         Device() const { return m_device; }
        VkCommandPool    CommandPool() const { return m_commandPool; }

        VkQueue GraphicsQueue() const { return m_graphicsQueue; }
        VkQueue PresentQueue() const { return m_presentQueue; }

        VkSampleCountFlagBits MsaaSamples() const { return m_msaaSamples; }

        VkShaderModule CreateShaderModule(std::vector<char> const& code) const;

    private:
        VkInstance            m_instance;
        VkPhysicalDevice      m_physicalDevice;
        VkDevice              m_device;
        VkQueue               m_graphicsQueue;
        VkQueue               m_presentQueue;
        VkSampleCountFlagBits m_msaaSamples;
        VkCommandPool         m_commandPool;
    };
}

#define VK_CHECK(result, msg) render::VulkanContext::VkCheck(result, msg)
