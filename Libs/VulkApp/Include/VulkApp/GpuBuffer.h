#pragma once

#include "VulkanUtils.h"

#include <vulkan/vulkan.h>

namespace render
{

class GpuBuffer
{
public:
    GpuBuffer();

    void Initialize(VulkanContext const& vkContext, void* const data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void CleanUp(VulkanContext const& vkContext);

    void Fill(VulkanContext const& vkContext, void* const data, uint64_t size);

    void Copy(VulkanContext const& vkContext, GpuBuffer& dstBuffer, VkDeviceSize size) const;

    template<typename T>
    T* Map(VulkanContext const& vkContext, uint64_t size) { void* tmp; vkMapMemory(vkContext.Device(), m_bufferMemory, 0, size, 0, &tmp);  return static_cast<T*>(tmp); }
    void UnMap(VulkanContext const& vkContext) { vkUnmapMemory(vkContext.Device(), m_bufferMemory); }

    VkBuffer Get() const { return m_buffer; }
    VkDeviceMemory BufferMemory() const { return m_bufferMemory; }

private:
    VkBuffer       m_buffer;
    VkDeviceMemory m_bufferMemory;
};

}
