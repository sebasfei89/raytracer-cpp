#pragma once

#include "Image.h"
#include "VulkanUtils.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <string>

namespace render
{
    class VulkanTexture
    {
    public:
        VulkanTexture();

        void Load(VulkanContext const& vkContext, std::string const& filePath);
        void Load(VulkanContext const& vkContext, unsigned char* pixels, int texWidth, int texHeight, int mipLevels);
        void Load(VulkanContext const& vkContext, glm::vec3 const& color, int width, int height);

        void Dispose(VulkanContext const& vkContext);

        VkDescriptorImageInfo GetImageInfo() const;
        VkImage GetImage() const { return m_textureImage.Image(); }

    protected:
        void CreateSampler(VkDevice device);

    private:
        VulkanImage    m_textureImage;
        VkSampler      m_textureSampler;
        uint32_t       m_mipLevels;
    };
}
