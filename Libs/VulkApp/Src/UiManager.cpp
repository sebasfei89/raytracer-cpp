#include "UiManager.h"

#include "VulkanUtils.h"

#include <imgui.h>

#include <array>

namespace
{
    static std::vector<int> ks_vertShaderCode = {
        0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
        0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
        0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
        0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
        0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
        0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
        0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
        0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
        0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
        0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
        0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
        0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
        0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
        0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
        0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
        0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
        0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
        0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
        0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
        0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
        0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
        0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
        0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
        0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
        0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
        0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
        0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
        0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
        0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
        0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
        0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
        0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
        0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
        0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
        0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
        0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
        0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
        0x0000002d,0x0000002c,0x000100fd,0x00010038
    };

    static std::vector<int> ks_fragShaderCode = {
        0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
        0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
        0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
        0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
        0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
        0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
        0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
        0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
        0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
        0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
        0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
        0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
        0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
        0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
        0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
        0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
        0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
        0x00010038
    };
}

UiManager::UiManager(render::VulkanContext* vkContext)
    : m_vkContext(vkContext)
    , m_fontTexture()
    , m_descriptorPool(VK_NULL_HANDLE)
    , m_descriptorSetLayout(VK_NULL_HANDLE)
    , m_descriptorSet(VK_NULL_HANDLE)
    , m_pipelineCache(VK_NULL_HANDLE)
    , m_pipelineLayout(VK_NULL_HANDLE)
    , m_pipeline(VK_NULL_HANDLE)
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_vertexCount(0)
    , m_indexCount(0)
    , m_pushConstBlock()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

UiManager::~UiManager()
{
    m_indexBuffer.CleanUp(*m_vkContext);
    m_vertexBuffer.CleanUp(*m_vkContext);

    vkDestroyPipelineCache(m_vkContext->Device(), m_pipelineCache, nullptr);

    vkDestroyDescriptorSetLayout(m_vkContext->Device(), m_descriptorSetLayout, nullptr);
    m_fontTexture.Dispose(*m_vkContext);

    ImGui::DestroyContext();

    m_vkContext = nullptr;
}

void UiManager::Initialize(float width, float height, float fbWidth, float fbHeight, VkRenderPass renderPass)
{
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Configure ImGui color scheme
    //ImGuiStyle& style = ImGui::GetStyle();
    //style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
    //style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    //style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    //style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    //style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.DisplaySize = ImVec2(width, height);
    io.DisplayFramebufferScale = ImVec2(fbWidth / width, fbHeight / height);

    // Create font texture
    if (m_fontTexture.GetImage() == VK_NULL_HANDLE)
    {
        unsigned char* fontData;
        int texWidth, texHeight;
        io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
        VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);
        m_fontTexture.Load(*m_vkContext, fontData, texWidth, texHeight, 1);
        io.Fonts->TexID = (ImTextureID)(intptr_t)m_fontTexture.GetImage();
    }

    {   // Create descriptor pool
        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 1;

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount = 1;
        descriptorPoolInfo.pPoolSizes = &poolSize;
        descriptorPoolInfo.maxSets = 2;

        VK_CHECK(vkCreateDescriptorPool(m_vkContext->Device(), &descriptorPoolInfo, nullptr, &m_descriptorPool), "Failed to create UiManager descriptor pool!");
    }

    // Create descriptor set layout
    if (m_descriptorSetLayout == VK_NULL_HANDLE)
    {
        VkDescriptorSetLayoutBinding setLayoutBinding = {};
        setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        setLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        setLayoutBinding.binding = 0;
        setLayoutBinding.descriptorCount = 1;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.pBindings = &setLayoutBinding;
        descriptorSetLayoutCreateInfo.bindingCount = 1;

        VK_CHECK(vkCreateDescriptorSetLayout(m_vkContext->Device(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout), "Failed to create UiManager descriptor set layout!");
    }

    {   // Create descriptor set
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.pSetLayouts = &m_descriptorSetLayout;
        allocInfo.descriptorSetCount = 1;

        VK_CHECK( vkAllocateDescriptorSets(m_vkContext->Device(), &allocInfo, &m_descriptorSet), "Failed to create UiManager descriptor set!" );

        VkDescriptorImageInfo fontDescriptor = m_fontTexture.GetImageInfo();
        VkWriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet = m_descriptorSet;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.pImageInfo = &fontDescriptor;
        writeDescriptorSet.descriptorCount = 1;

        vkUpdateDescriptorSets(m_vkContext->Device(), 1, &writeDescriptorSet, 0, nullptr);
    }

    // Create pipeline cache
    if (m_pipelineCache == VK_NULL_HANDLE)
    {
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        VK_CHECK( vkCreatePipelineCache(m_vkContext->Device(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache), "Failed to create UiManager pipeline cache!" );
    }

    // Create pipeline layout
    {
        // Push constants for UI rendering parameters
        VkPushConstantRange pushConstantRange = {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstBlock);

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 1;
        pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
        pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

        VK_CHECK( vkCreatePipelineLayout(m_vkContext->Device(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout), "Failed to create UiManager pipeline layout!" );
    }

    CreateGraphicsPipeline(renderPass);
}

void UiManager::CreateGraphicsPipeline(VkRenderPass renderPass)
{
    // Setup graphics pipeline for UI rendering
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.flags = 0;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.flags = 0;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.lineWidth = 1.0f;

    // Enable blending
    VkPipelineColorBlendAttachmentState blendAttachmentState = {};
    blendAttachmentState.blendEnable = VK_TRUE;
    blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentState;

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_FALSE;
    depthStencilState.depthWriteEnable = VK_FALSE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilState.front = depthStencilState.back;
    depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.flags = 0;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = m_vkContext->MsaaSamples();
    multisampleState.flags = 0;

    std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    dynamicState.flags = 0;

    size_t shaderSize = ks_vertShaderCode.size() * sizeof(int);
    std::vector<char> shaderCode;
    shaderCode.resize(shaderSize);
    std::memcpy(shaderCode.data(), ks_vertShaderCode.data(), shaderSize);
    VkShaderModule vertShaderModule = m_vkContext->CreateShaderModule(shaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    shaderSize = ks_fragShaderCode.size() * sizeof(int);
    shaderCode.resize(shaderSize);
    std::memcpy(shaderCode.data(), ks_fragShaderCode.data(), shaderSize);
    VkShaderModule fragShaderModule = m_vkContext->CreateShaderModule(shaderCode);

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = m_pipelineLayout;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.basePipelineIndex = -1;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineCreateInfo.pStages = shaderStages.data();

    // Vertex bindings an attributes based on ImGui vertex definition
    VkVertexInputBindingDescription vertexInputBindings = {};
    vertexInputBindings.binding = 0;
    vertexInputBindings.stride = sizeof(ImDrawVert);
    vertexInputBindings.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
        { 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos) }, // Location 0: Position
        { 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv) },  // Location 1: UV
        { 2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col) } // Location 2: Color
    };
    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBindings;
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputState;

    VK_CHECK( vkCreateGraphicsPipelines(m_vkContext->Device(), m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline), "Failed to create UiManager graphics pipeline!" );

    vkDestroyShaderModule(m_vkContext->Device(), vertShaderModule, nullptr);
    vkDestroyShaderModule(m_vkContext->Device(), fragShaderModule, nullptr);
}

void UiManager::CleanupSwapChainResources()
{
    vkDestroyPipeline(m_vkContext->Device(), m_pipeline, nullptr);
    m_pipeline = VK_NULL_HANDLE;
    vkDestroyPipelineLayout(m_vkContext->Device(), m_pipelineLayout, nullptr);
    m_pipelineLayout = VK_NULL_HANDLE;
    vkDestroyDescriptorPool(m_vkContext->Device(), m_descriptorPool, nullptr);
    m_descriptorPool = VK_NULL_HANDLE;
}

void UiManager::BuildCommandBuffer(VkCommandBuffer cmdBuffer, float deltaTime)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = deltaTime;

    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    VkViewport viewport{};
    viewport.width = io.DisplaySize.x;
    viewport.height = io.DisplaySize.y;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    // UI scale and translate via push constants
    m_pushConstBlock.scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
    m_pushConstBlock.translate = glm::vec2(-1.0f);
    vkCmdPushConstants(cmdBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock), &m_pushConstBlock);

    // Render commands
    ImDrawData* imDrawData = ImGui::GetDrawData();
    int32_t vertexOffset = 0;
    int32_t indexOffset = 0;

    if ((imDrawData != nullptr) && (imDrawData->CmdListsCount > 0))
    {

        VkDeviceSize offsets[1] = { 0 };
        VkBuffer vertBuffer = m_vertexBuffer.Get();
        vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertBuffer, offsets);
        vkCmdBindIndexBuffer(cmdBuffer, m_indexBuffer.Get(), 0, VK_INDEX_TYPE_UINT16);

        for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
        {
            const ImDrawList* cmd_list = imDrawData->CmdLists[i];
            for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
                VkRect2D scissorRect;
                scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
                scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
                scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
                vkCmdSetScissor(cmdBuffer, 0, 1, &scissorRect);
                vkCmdDrawIndexed(cmdBuffer, pcmd->ElemCount, 1, indexOffset, vertexOffset, 0);
                indexOffset += pcmd->ElemCount;
            }
            vertexOffset += cmd_list->VtxBuffer.Size;
        }
    }
}

bool UiManager::UpdateMousePos(bool windowFocus, uint8_t mouseButtons, double& x, double& y)
{
    static bool s_MouseJustPressed[5] = { false, false, false, false, false };

    // Update buttons
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = s_MouseJustPressed[i] || (mouseButtons & (1 << i)) != 0;
        s_MouseJustPressed[i] = false;
    }

    // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    if (windowFocus)
    {
        if (io.WantSetMousePos)
        {
            x = (double)mouse_pos_backup.x;
            y = (double)mouse_pos_backup.y;
            return true;
        }
        else
        {
            io.MousePos = ImVec2((float)x, (float)y);
        }
    }
    return false;
}

void UiManager::Render()
{
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    UpdateBuffers();
}

void UiManager::UpdateBuffers()
{
    ImDrawData* imDrawData = ImGui::GetDrawData();

    // Note: Alignment is done inside buffer creation
    VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
    VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

    if ((vertexBufferSize == 0) || (indexBufferSize == 0)) {
        return;
    }

    // Update buffers only if vertex or index count has been changed compared to current buffer size

    // Vertex buffer
    if (m_vertexCount != imDrawData->TotalVtxCount)
    {
        VK_CHECK( vkQueueWaitIdle(m_vkContext->PresentQueue()), "Failed to wait for idle on present queue!");
        m_vertexBuffer.CleanUp(*m_vkContext);
        m_vertexBuffer.Initialize(*m_vkContext, nullptr, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        m_vertexCount = imDrawData->TotalVtxCount;
    }

    // Index buffer
    VkDeviceSize indexSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);
    if (m_indexCount < (uint32_t)imDrawData->TotalIdxCount)
    {
        VK_CHECK(vkQueueWaitIdle(m_vkContext->PresentQueue()), "Failed to wait for idle on present queue!");
        m_indexBuffer.CleanUp(*m_vkContext);
        m_indexBuffer.Initialize(*m_vkContext, nullptr, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        m_indexCount = imDrawData->TotalIdxCount;
    }

    // Upload data
    ImDrawVert* vtxDst = m_vertexBuffer.Map<ImDrawVert>(*m_vkContext, vertexBufferSize);
    ImDrawIdx* idxDst = m_indexBuffer.Map<ImDrawIdx>(*m_vkContext, indexBufferSize);

    for (int n = 0; n < imDrawData->CmdListsCount; n++) {
        const ImDrawList* cmd_list = imDrawData->CmdLists[n];
        memcpy(vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
        memcpy(idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        vtxDst += cmd_list->VtxBuffer.Size;
        idxDst += cmd_list->IdxBuffer.Size;
    }

    // Flush to make writes visible to GPU
    m_vertexBuffer.UnMap(*m_vkContext);
    m_indexBuffer.UnMap(*m_vkContext);
}
