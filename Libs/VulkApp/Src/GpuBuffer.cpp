#include "GpuBuffer.h"

namespace render
{

GpuBuffer::GpuBuffer()
    : m_buffer(VK_NULL_HANDLE)
    , m_bufferMemory(VK_NULL_HANDLE)
{
}

void GpuBuffer::Initialize(VulkanContext const& vkContext, void* const data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(vkContext.Device(), &bufferInfo, nullptr, &m_buffer), "Failed to create gpu buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkContext.Device(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vkContext.FindMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(vkContext.Device(), &allocInfo, nullptr, &m_bufferMemory), "Failed to allocate gpu buffer memory!");
    vkBindBufferMemory(vkContext.Device(), m_buffer, m_bufferMemory, 0);

    if (data != nullptr)
    {
        Fill(vkContext, data, size);
    }
}

void GpuBuffer::CleanUp(VulkanContext const& vkContext)
{
    vkDestroyBuffer(vkContext.Device(), m_buffer, nullptr);
    m_buffer = VK_NULL_HANDLE;
    vkFreeMemory(vkContext.Device(), m_bufferMemory, nullptr);
    m_bufferMemory = VK_NULL_HANDLE;
}

void GpuBuffer::Fill(VulkanContext const& vkContext, void* const data, uint64_t size)
{
    void* tmp = Map<void>(vkContext, size);
    memcpy(tmp, data, static_cast<size_t>(size));
    UnMap(vkContext);
}

void GpuBuffer::Copy(VulkanContext const& vkContext, GpuBuffer& dstBuffer, VkDeviceSize size) const
{
    VkCommandBuffer commandBuffer = vkContext.BeginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, m_buffer, dstBuffer.Get(), 1, &copyRegion);

    vkContext.EndSingleTimeCommands(commandBuffer);
}

}
