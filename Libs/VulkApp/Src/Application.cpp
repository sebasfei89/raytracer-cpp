#include "Application.h"
#include "UiManager.h"

// ThirdParty
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// stl
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
    static void FramebufferResizeCB(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
        app->OnFrameBufferResized();
    }

    static std::vector<char> ReadFile(std::string const& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    constexpr int WINDOW_WIDTH = 1200;
    constexpr int WINDOW_HEIGHT = 800;

    constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    std::string const MODEL_PATH = "Resources/Models/chalet.obj";
    std::string const TEXTURE_PATH = "Resources/Textures/chalet.jpg";

#ifdef NDEBUG
    constexpr bool kValidationLayersEnabled = false;
#else
    constexpr bool kValidationLayersEnabled = true;
#endif

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cerr << "[Vulkan] Validation layer error: " << pCallbackData->pMessage << std::endl;
        }
        else
        {
            std::cout << "[Vulkan] Validation layer info: " << pCallbackData->pMessage << std::endl;
        }
        return VK_FALSE;
    }

    static void GlfwErrorCallback(int error, char const* description)
    {
        std::cerr << "[GLFW] Error " << error << ": " << description << std::endl;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const* pCreateInfo, VkAllocationCallbacks const* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        if (auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"))
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, VkAllocationCallbacks const* pAllocator)
    {
        if (auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))
        {
            func(instance, debugMessenger, pAllocator);
        }
    }
}

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return (hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec2>()(vertex.texCoord) << 1));
        }
    };
}

VulkanApplication::VulkanApplication()
    : m_window(nullptr)
    , m_debugMessenger(VK_NULL_HANDLE)
    , m_surface(VK_NULL_HANDLE)
    , m_renderPass(VK_NULL_HANDLE)
    , m_descriptorSetLayout(VK_NULL_HANDLE)
    , m_pipelineLayout(VK_NULL_HANDLE)
    , m_graphicsPipeline(VK_NULL_HANDLE)
    , m_commandBuffers()
    , m_imageAvailableSemaphores()
    , m_renderFinishedSemaphores()
    , m_inFlightFences()
    , m_framebufferResized(false)
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_uniformBuffers()
    , m_descriptorPool(VK_NULL_HANDLE)
    , m_descriptorSets()
    , m_texture()
    , m_vertices()
    , m_indices()
    , m_uiManager(nullptr)
{
}

bool VulkanApplication::Init()
{
    InitWindow();
    LoadModel();
    InitVulkan();
    return true;
}

void VulkanApplication::Run()
{
    MainLoop();
    CleanUp();
}

void VulkanApplication::InitWindow()
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Tutorial", nullptr, nullptr);
    if (glfwVulkanSupported() != GLFW_TRUE)
    {
        throw std::runtime_error("GLFW: Vulkan Not Supported");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCB);
}

void VulkanApplication::MainLoop()
{
    size_t currentFrame = 0;
    double lastTime = 0.0f;

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        // Setup time step
        double currentTime = glfwGetTime();
        float deltaTime = (lastTime > 0.0) ? (float)(currentTime - lastTime) : (1.0f / 60.0f);
        lastTime = currentTime;

        { // Handle input
            bool const focused = glfwGetWindowAttrib(m_window, GLFW_FOCUSED) != 0;
            double mouse_x, mouse_y;
            glfwGetCursorPos(m_window, &mouse_x, &mouse_y);

            uint8_t mouseButtons = 0;
            for (int i = 0; i < 5; i++)
            {
                if (glfwGetMouseButton(m_window, i) == GLFW_PRESS)
                {
                    mouseButtons |= (1 << i);
                }
            }

            if (m_uiManager->UpdateMousePos(focused, mouseButtons, mouse_x, mouse_y))
            {
                glfwSetCursorPos(m_window, mouse_x, mouse_y);
            }
        }

        DrawFrame(currentFrame, deltaTime);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    m_vkContext.WaitDeviceIdle();
}

void VulkanApplication::CleanUp()
{
    CleanupSwapChain();

    delete m_uiManager;
    m_uiManager = nullptr;

    // Texture & sampler resources
    m_texture.Dispose(m_vkContext);

    vkDestroyDescriptorSetLayout(m_vkContext.Device(), m_descriptorSetLayout, nullptr);

    m_indexBuffer.CleanUp(m_vkContext);
    m_vertexBuffer.CleanUp(m_vkContext);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_vkContext.Device(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(m_vkContext.Device(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(m_vkContext.Device(), m_inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_vkContext.Device(), m_vkContext.CommandPool(), nullptr);

    vkDestroyDevice(m_vkContext.Device(), nullptr);
    vkDestroySurfaceKHR(m_vkContext.Instance(), m_surface, nullptr);
    if (kValidationLayersEnabled)
    {
        DestroyDebugUtilsMessengerEXT(m_vkContext.Instance(), m_debugMessenger, nullptr);
    }
    vkDestroyInstance(m_vkContext.Instance(), nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void VulkanApplication::InitVulkan()
{
    // Step 1 - Instance creation and physical device selection (VkInstance, VkPhysicalDevice)
    m_vkContext.CreateInstance(kValidationLayersEnabled);
    SetupDebugMessenger();
    VK_CHECK(glfwCreateWindowSurface(m_vkContext.Instance(), m_window, nullptr, &m_surface), "Failed to create window surface!");
    m_vkContext.PickPhysicalDevice(m_surface);
    // Step 2 - Logical device and queue families (VkDevice, VkQueue)
    m_vkContext.CreateLogicalDevice(kValidationLayersEnabled, m_surface);

    // Step 3 - Window surface and swap chain (VkSurfaceKHR, VkSwapchainKHR) OR offscreen rendering setup (VK_KHR_display, VK_KHR_display_swapchain)
    // Step 4 - Image views (VkImageView)
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    m_swapChain.Initialize(m_vkContext, m_surface, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

    // Step 5 - Render passes
    CreateRenderPass();

    // Step 6 - Graphics pipeline (VkPipeline, VkShaderModule)
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();

    // Step 7 - Command pools and command buffers (VkCommandPool, VkCommandBuffer)
    m_vkContext.CreateCommandPool(m_surface);

    m_uiManager = new UiManager(&m_vkContext);
    m_uiManager->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, (float)width, (float)height, m_renderPass);

    // Step 4 - Framebuffers (VkFramebuffer)
    m_swapChain.CreateResources(m_vkContext, m_renderPass);
    m_texture.Load(m_vkContext, glm::vec3(1.f, 0.f, 0.f), 1920, 1080);

    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();

    // Step 8 - Main loop (vkAcquireNextImageKHR, vkQueueSubmit, vkQueuePresentKHR)
    CreateSyncObjects();
}

void VulkanApplication::SetupDebugMessenger()
{
    if (!kValidationLayersEnabled) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
    createInfo.pUserData = nullptr;

    VK_CHECK( CreateDebugUtilsMessengerEXT(m_vkContext.Instance(), &createInfo, nullptr, &m_debugMessenger), "Failed to setup debug messenger!");
}

void VulkanApplication::CreateGraphicsPipeline()
{
    auto const vertShaderCode = ReadFile("Resources/Shaders/main.vert.glsl.spv");
    auto const fragShaderCode = ReadFile("Resources/Shaders/main.frag.glsl.spv");

    // Creating shader modules
    VkShaderModule const vertShaderModule = m_vkContext.CreateShaderModule(vertShaderCode);
    VkShaderModule const fragShaderModule = m_vkContext.CreateShaderModule(fragShaderCode);

    // Shader stage creation
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // Vertex input
    auto const bindingDescription = Vertex::GetBindingDescription();
    auto const attributeDescriptions = Vertex::GetAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewports and scissors
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_swapChain.Extent().width;
    viewport.height = (float)m_swapChain.Extent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapChain.Extent();

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = m_vkContext.MsaaSamples();
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // Depth and stencil testing
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};

    // Dynamic state
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    // Pipeline layout
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        VK_CHECK( vkCreatePipelineLayout(m_vkContext.Device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout), "Failed to create pipeline layout!" );
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // Shader stages
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    // Vertex input
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    // Input assembly
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    // Viewports and scissors
    pipelineInfo.pViewportState = &viewportState;
    // Rasterizer
    pipelineInfo.pRasterizationState = &rasterizer;
    // Multisampling
    pipelineInfo.pMultisampleState = &multisampling;
    // Color blending
    pipelineInfo.pColorBlendState = &colorBlending;
    // Depth and stencil testing
    pipelineInfo.pDepthStencilState = &depthStencil;
    // Dynamic state
    pipelineInfo.pDynamicState = nullptr;
    // Pipeline layout
    pipelineInfo.layout = m_pipelineLayout;
    // Render pass
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    // Base pipeline
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    VK_CHECK( vkCreateGraphicsPipelines(m_vkContext.Device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline), "Failed to create graphics pipeline!" );

    vkDestroyShaderModule(m_vkContext.Device(), fragShaderModule, nullptr);
    vkDestroyShaderModule(m_vkContext.Device(), vertShaderModule, nullptr);
}

void VulkanApplication::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapChain.ImageFormat();
    colorAttachment.samples = m_vkContext.MsaaSamples();
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve = {};
    colorAttachmentResolve.format = m_swapChain.ImageFormat();
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef = {};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = m_swapChain.FindDepthFormat(m_vkContext.PhysicalDevice());
    depthAttachment.samples = m_vkContext.MsaaSamples();
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VK_CHECK( vkCreateRenderPass(m_vkContext.Device(), &renderPassInfo, nullptr, &m_renderPass), "Failed to create render pass!");
}

void VulkanApplication::CreateCommandBuffers()
{
    m_commandBuffers.resize(m_swapChain.FramebuffersCount());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_vkContext.CommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

    VK_CHECK( vkAllocateCommandBuffers(m_vkContext.Device(), &allocInfo, m_commandBuffers.data()), "Failed to allocate command buffers!" );
}

void VulkanApplication::BuildCommandBuffer(uint32_t imageIdx, float deltaTime)
{
    static std::array<VkClearValue, 2> clearValues = {VkClearValue{{ 0.0f, 0.0f, 0.0f, 1.0f }}, VkClearValue{{1.0f, 0}}};

    //m_uiManager->Render();

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapChain.Extent();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());;
    renderPassInfo.pClearValues = clearValues.data();

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr; // Optional
    VK_CHECK( vkBeginCommandBuffer(m_commandBuffers[imageIdx], &beginInfo), "Failed to begin recording command buffer!" );

    renderPassInfo.framebuffer = m_swapChain.Framebuffer(imageIdx);

    // Render pass
    vkCmdBeginRenderPass(m_commandBuffers[imageIdx], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Draw our model
    vkCmdBindPipeline(m_commandBuffers[imageIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    vkCmdBindDescriptorSets(m_commandBuffers[imageIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSets[imageIdx], 0, nullptr);
    VkBuffer vertexBuffers[] = { m_vertexBuffer.Get() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(m_commandBuffers[imageIdx], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(m_commandBuffers[imageIdx], m_indexBuffer.Get(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(m_commandBuffers[imageIdx], static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);

    m_uiManager->BuildCommandBuffer(m_commandBuffers[imageIdx], deltaTime);

    vkCmdEndRenderPass(m_commandBuffers[imageIdx]);

    VK_CHECK( vkEndCommandBuffer(m_commandBuffers[imageIdx]), "Failed to record command buffer!" );
}

void VulkanApplication::CreateSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VK_CHECK( vkCreateSemaphore(m_vkContext.Device(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]), "Failed to create image available semaphore!" );
        VK_CHECK( vkCreateSemaphore(m_vkContext.Device(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]), "Failed to create render finished semaphore!" );
        VK_CHECK( vkCreateFence(m_vkContext.Device(), &fenceInfo, nullptr, &m_inFlightFences[i]), "Failed to create fences!" );
    }
}

void VulkanApplication::DrawFrame(size_t currentFrame, float deltaTime)
{
    VkResult result = vkWaitForFences(m_vkContext.Device(), 1, &m_inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
    if (result != VK_SUCCESS)
    {
        if (result == VK_TIMEOUT)
        {
            std::cout << "vkWaitForFences timed out!";
        }
        else
        {
            std::cerr << "vkWaitForFences failed!";
        }
    }

    uint32_t imageIndex;
    if (!m_swapChain.AcquireNextImage(m_vkContext, m_imageAvailableSemaphores[currentFrame], &imageIndex))
    {
        RecreateSwapChain();
        return;
    }

    UpdateUniformBuffer(imageIndex);

    BuildCommandBuffer(imageIndex, deltaTime);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_vkContext.Device(), 1, &m_inFlightFences[currentFrame]);
    VK_CHECK( vkQueueSubmit(m_vkContext.GraphicsQueue(), 1, &submitInfo, m_inFlightFences[currentFrame]), "Failed to submit draw command buffer!" );

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = { m_swapChain.Get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(m_vkContext.PresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized)
    {
        m_framebufferResized = false;
        RecreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}

void VulkanApplication::RecreateSwapChain()
{
    int width = 0;
    int height = 0;
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    m_vkContext.WaitDeviceIdle();

    CleanupSwapChain();

    m_swapChain.Initialize(m_vkContext, m_surface, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

    CreateRenderPass();
    CreateGraphicsPipeline();
    m_swapChain.CreateResources(m_vkContext, m_renderPass);
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();

    int winWidth, winHeight;
    glfwGetWindowSize(m_window, &winWidth, &winHeight);
    m_uiManager->Initialize((float)winWidth, (float)winHeight, (float)width, (float)height, m_renderPass);

    CreateCommandBuffers();
}

void VulkanApplication::CleanupSwapChain()
{
    m_swapChain.CleanupResources(m_vkContext);

    vkFreeCommandBuffers(m_vkContext.Device(), m_vkContext.CommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

    vkDestroyPipeline(m_vkContext.Device(), m_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_vkContext.Device(), m_pipelineLayout, nullptr);
    vkDestroyRenderPass(m_vkContext.Device(), m_renderPass, nullptr);

    m_swapChain.Finalize(m_vkContext);

    for (size_t i = 0; i < m_swapChain.ImageCount(); i++)
    {
        m_uniformBuffers[i].CleanUp(m_vkContext);
    }

    vkDestroyDescriptorPool(m_vkContext.Device(), m_descriptorPool, nullptr);

    m_uiManager->CleanupSwapChainResources();
}

void VulkanApplication::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

    render::GpuBuffer stagingBuffer;
    stagingBuffer.Initialize(m_vkContext, m_vertices.data(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    m_vertexBuffer.Initialize(m_vkContext, nullptr, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    stagingBuffer.Copy(m_vkContext, m_vertexBuffer, bufferSize);

    stagingBuffer.CleanUp(m_vkContext);
}

void VulkanApplication::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

    render::GpuBuffer stagingBuffer;
    stagingBuffer.Initialize(m_vkContext, m_indices.data(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    m_indexBuffer.Initialize(m_vkContext, nullptr, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    stagingBuffer.Copy(m_vkContext, m_indexBuffer, bufferSize);

    stagingBuffer.CleanUp(m_vkContext);
}

void VulkanApplication::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VK_CHECK( vkCreateDescriptorSetLayout(m_vkContext.Device(), &layoutInfo, nullptr, &m_descriptorSetLayout), "Failed to create descriptor set layout!" );
}

void VulkanApplication::CreateUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    m_uniformBuffers.resize(m_swapChain.ImageCount());
    for (size_t i = 0; i < m_swapChain.ImageCount(); i++)
    {
        m_uniformBuffers[i].Initialize(m_vkContext, nullptr, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
}

void VulkanApplication::UpdateUniformBuffer(uint32_t imageIndex)
{
    UniformBufferObject ubo = {};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(-1.f, 1.f, 1.f, -1.f, 0.f, 1.f);
    m_uniformBuffers[imageIndex].Fill(m_vkContext, &ubo, sizeof(ubo));
}

void VulkanApplication::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(m_swapChain.ImageCount());
    
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(m_swapChain.ImageCount());

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(m_swapChain.ImageCount());

    VK_CHECK( vkCreateDescriptorPool(m_vkContext.Device(), &poolInfo, nullptr, &m_descriptorPool), "Failed to create descriptor pool!" );
}

void VulkanApplication::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(m_swapChain.ImageCount(), m_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapChain.ImageCount());
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(m_swapChain.ImageCount());
    VK_CHECK( vkAllocateDescriptorSets(m_vkContext.Device(), &allocInfo, m_descriptorSets.data()), "Failed to allocate descriptor sets!" );

    UpdateDescriptorSets();
}

void VulkanApplication::UpdateDescriptorSets()
{
    for (size_t i = 0; i < m_swapChain.ImageCount(); i++)
    {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = m_uniformBuffers[i].Get();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = m_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        VkDescriptorImageInfo imageInfo = m_texture.GetImageInfo();
        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = m_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(m_vkContext.Device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void VulkanApplication::LoadModel()
{
    m_vertices.push_back({ { -1.f,  1.f, 1.f }, { 0.0f, 0.0f } });
    m_vertices.push_back({ {  1.f,  1.f, 1.f }, { 1.0f, 0.0f } });
    m_vertices.push_back({ {  1.f, -1.f, 1.f }, { 1.0f, 1.0f } });
    m_vertices.push_back({ { -1.f, -1.f, 1.f }, { 0.0f, 1.0f } });

    m_indices.push_back(0); m_indices.push_back(1); m_indices.push_back(2);
    m_indices.push_back(2); m_indices.push_back(3); m_indices.push_back(0);
}

void VulkanApplication::UpdateTexture(unsigned char* pixels, uint32_t width, uint32_t height)
{
    m_texture.Dispose(m_vkContext);
    m_texture.Load(m_vkContext, pixels, width, height, 1);
    UpdateDescriptorSets();
}
