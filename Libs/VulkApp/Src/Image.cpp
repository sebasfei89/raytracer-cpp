#include "Image.h"

namespace render
{

VulkanImage::VulkanImage()
    : m_image(VK_NULL_HANDLE)
    , m_imageMemory(VK_NULL_HANDLE)
    , m_imageView(VK_NULL_HANDLE)
    , m_imageOwned(false)
{
}

void VulkanImage::Initialize(VulkanContext const& vkContext, VkExtent2D extent, uint32_t mipLevels,
                             VkSampleCountFlagBits samples, VkFormat format, VkImageUsageFlags usageFlags,
                             VkImageAspectFlags aspectFlags, VkImageLayout layout)
{
    m_imageOwned = true;

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usageFlags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = samples;
    imageInfo.flags = 0;

    VK_CHECK(vkCreateImage(vkContext.Device(), &imageInfo, nullptr, &m_image), "Failed to create image!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkContext.Device(), m_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vkContext.FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK(vkAllocateMemory(vkContext.Device(), &allocInfo, nullptr, &m_imageMemory), "Failed to allocate image memory!");

    vkBindImageMemory(vkContext.Device(), m_image, m_imageMemory, 0);

    m_imageView = vkContext.CreateImageView(m_image, format, aspectFlags, mipLevels);
    vkContext.TransitionImageLayout(m_image, format, VK_IMAGE_LAYOUT_UNDEFINED, layout, mipLevels);
}

void VulkanImage::Initialize(VulkanContext const& vkContext, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
    m_imageOwned = false;
    m_image = image;
    m_imageView = vkContext.CreateImageView(m_image, format, aspectFlags, mipLevels);
}

void VulkanImage::CleanUp(VulkanContext const& vkContext)
{
    vkDestroyImageView(vkContext.Device(), m_imageView, nullptr);
    if (m_imageOwned)
    {
        vkDestroyImage(vkContext.Device(), m_image, nullptr);
        vkFreeMemory(vkContext.Device(), m_imageMemory, nullptr);
    }
}

}
