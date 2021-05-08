#include "SwapChain.h"

#include <algorithm>
#include <array>

namespace
{
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& availableFormats)
    {
        if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        }

        for (auto const& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode(std::vector<VkPresentModeKHR> const& availablePresentModes)
    {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
        for (auto const& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
            else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                bestMode = availablePresentMode;
            }
        }
        return bestMode;
    }

    VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities, uint32_t width, uint32_t height)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = { width, height };
            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
            return actualExtent;
        }
    }

    VkFormat FindSupportedFormat(VkPhysicalDevice vkPhysicalDevice, std::vector<VkFormat> const& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }
        throw std::runtime_error("Failed to find supported format!");
    }
} // Anonymous namespace

namespace render
{

VulkanSwapChain::VulkanSwapChain()
    : m_swapChain(VK_NULL_HANDLE)
    , m_swapChainImages()
    , m_swapChainFramebuffers()
    , m_swapChainImageFormat(VK_FORMAT_UNDEFINED)
    , m_extent{ 0,0 }
    , m_colorImage()
    , m_depthImage()
{
}

void VulkanSwapChain::Initialize(VulkanContext const& vkContext, VkSurfaceKHR surface, uint32_t width, uint32_t height)
{
    SwapChainSupportDetails const swapChainSupport = QuerySwapChainSupport(vkContext.PhysicalDevice(), surface);
    VkSurfaceFormatKHR const surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.m_formats);
    VkPresentModeKHR const presentMode = ChooseSwapPresentMode(swapChainSupport.m_presentModes);
    m_extent = ChooseSwapExtent(swapChainSupport.m_capabilities, width, height);
    m_swapChainImageFormat = surfaceFormat.format;

    uint32_t imageCount = swapChainSupport.m_capabilities.minImageCount + 1;
    if (swapChainSupport.m_capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.m_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = m_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    render::QueueFamilyIndices indices = render::FindQueueFamilies(vkContext.PhysicalDevice(), surface);
    uint32_t queueFamilyIndices[] = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };
    if (indices.m_graphicsFamily != indices.m_presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.m_capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK(vkCreateSwapchainKHR(vkContext.Device(), &createInfo, nullptr, &m_swapChain), "Failed to create swap chain!");

    std::vector<VkImage> images;
    vkGetSwapchainImagesKHR(vkContext.Device(), m_swapChain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(vkContext.Device(), m_swapChain, &imageCount, images.data());

    m_swapChainImages.resize(imageCount);
    for (size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        m_swapChainImages[i].Initialize(vkContext, images[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

void VulkanSwapChain::CreateFramebuffers(VulkanContext const& vkContext, VkRenderPass renderPass)
{
    m_swapChainFramebuffers.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++)
    {
        std::array<VkImageView, 3> attachments = { m_colorImage.ImageView(), m_depthImage.ImageView(), m_swapChainImages[i].ImageView() };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_extent.width;
        framebufferInfo.height = m_extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(vkContext.Device(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]), "Failed to create framebuffer!");
    }
}

bool VulkanSwapChain::AcquireNextImage(VulkanContext const& vkContext, VkSemaphore semaphore, uint32_t* imageIndex)
{
    VkResult result = vkAcquireNextImageKHR(vkContext.Device(), m_swapChain, std::numeric_limits<uint64_t>::max(), semaphore, VK_NULL_HANDLE, imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return false;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    return true;
}

void VulkanSwapChain::Finalize(VulkanContext const& vkContext)
{
    for (auto& image : m_swapChainImages)
    {
        image.CleanUp(vkContext);
    }
    vkDestroySwapchainKHR(vkContext.Device(), m_swapChain, nullptr);
}

void VulkanSwapChain::CreateResources(VulkanContext const& vkContext, VkRenderPass renderPass)
{
    CreateColorResources(vkContext);
    CreateDepthResources(vkContext);
    CreateFramebuffers(vkContext, renderPass);
}

void VulkanSwapChain::CleanupResources(VulkanContext const& vkContext)
{
    m_colorImage.CleanUp(vkContext);
    m_depthImage.CleanUp(vkContext);

    for (auto framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(vkContext.Device(), framebuffer, nullptr);
    }
}

void VulkanSwapChain::CreateColorResources(VulkanContext const& vkContext)
{
    VkFormat colorFormat = m_swapChainImageFormat;
    m_colorImage.Initialize(vkContext, m_extent, 1, vkContext.MsaaSamples(), colorFormat,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
}

void VulkanSwapChain::CreateDepthResources(VulkanContext const& vkContext)
{
    VkFormat depthFormat = FindDepthFormat(vkContext.PhysicalDevice());
    m_depthImage.Initialize(vkContext, m_extent, 1, vkContext.MsaaSamples(), depthFormat,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

VkFormat VulkanSwapChain::FindDepthFormat(VkPhysicalDevice vkPhysicalDevice) const
{
    return FindSupportedFormat(vkPhysicalDevice, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

} // namespace render
