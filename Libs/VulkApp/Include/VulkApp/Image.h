#pragma once

#include "VulkanUtils.h"

namespace render
{

class VulkanImage
{
public:
    VulkanImage();

    // Un-owned VkImage
    void Initialize(VulkanContext const& vkContext, VkImage image, VkFormat format,
        VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    void Initialize(VulkanContext const& vkContext, VkExtent2D extent, uint32_t mipLevels,
        VkSampleCountFlagBits samples, VkFormat format, VkImageUsageFlags usageFlags,
        VkImageAspectFlags aspectFlags, VkImageLayout layout);

    void CleanUp(VulkanContext const& vkContext);

    VkImage     Image() const { return m_image; }
    VkImageView ImageView() const { return m_imageView; }

private:
    VkImage         m_image;
    VkDeviceMemory  m_imageMemory;
    VkImageView     m_imageView;
    bool            m_imageOwned;
};

}
