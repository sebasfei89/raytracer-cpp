#include "Texture.h"

#include "GpuBuffer.h"
#include "VulkanUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>

namespace render
{

VulkanTexture::VulkanTexture()
    : m_textureImage()
    , m_textureSampler(VK_NULL_HANDLE)
    , m_mipLevels(1)
{
}

void VulkanTexture::Load(VulkanContext const& vkContext, glm::vec3 const& color, int width, int height)
{
    unsigned char *pixels = new unsigned char[width * height * 4];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            pixels[(h * width * 4) + (w * 4)] = static_cast<unsigned char>(color.r * 255.f);
            pixels[(h * width * 4) + (w * 4) + 1] = static_cast<unsigned char>(color.g * 255.f);
            pixels[(h * width * 4) + (w * 4) + 2] = static_cast<unsigned char>(color.b * 255.f);
            pixels[(h * width * 4) + (w * 4) + 3] = 255;
        }
    }
    Load(vkContext, pixels, width, height, 1);
    delete[] pixels;
}

void VulkanTexture::Load(VulkanContext const& vkContext, std::string const& filePath)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels)
    {
        throw std::runtime_error("Failed to load texture image!");
    }

    int mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    Load(vkContext, pixels, texWidth, texHeight, mipLevels);

    stbi_image_free(pixels);
}

void VulkanTexture::Load(VulkanContext const& vkContext, unsigned char* pixels, int texWidth, int texHeight, int mipLevels)
{
    m_mipLevels = mipLevels;
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    GpuBuffer stagingBuffer;
    stagingBuffer.Initialize(vkContext, pixels, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkExtent2D extent = { static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) };
    m_textureImage.Initialize(vkContext, extent, m_mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vkContext.CopyBufferToImage(stagingBuffer.Get(), m_textureImage.Image(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    if (m_mipLevels > 1)
    {
        vkContext.GenerateMipmaps(m_textureImage.Image(), VK_FORMAT_R8G8B8A8_UNORM, texWidth, texHeight, m_mipLevels);
    }
    else
    {
        vkContext.TransitionImageLayout(m_textureImage.Image(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
    }

    stagingBuffer.CleanUp(vkContext);

    CreateSampler(vkContext.Device());
}

void VulkanTexture::Dispose(VulkanContext const& vkContext)
{
    vkDestroySampler(vkContext.Device(), m_textureSampler, nullptr);
    m_textureImage.CleanUp(vkContext);
}

void VulkanTexture::CreateSampler(VkDevice device)
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(m_mipLevels);

    VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &m_textureSampler), "[VulkanTexture] Failed to create sampler!");
}

VkDescriptorImageInfo VulkanTexture::GetImageInfo() const
{
    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_textureImage.ImageView();
    imageInfo.sampler = m_textureSampler;
    return imageInfo;
}

} // namespace render
