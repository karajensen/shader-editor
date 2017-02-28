////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkaninit.h"
#include "vulkandata.h"
#include "vulkanutils.h"
#include "vulkanshader.h"
#include "sceneInterface.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include <array>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

VulkanEngine::VulkanEngine(HWND hwnd, HINSTANCE hinstance) :
    m_data(new VulkanData(hinstance, hwnd))
{
}

VulkanEngine::~VulkanEngine()
{
    Release();
}

void VulkanEngine::Release()
{
    m_data->Release();
}

bool VulkanEngine::Initialize()
{
    auto& info = *m_data;

    if (CHECK_FAIL(VulkanInit::InitGlobalLayerProperties(info)) ||
        CHECK_FAIL(VulkanInit::InitInstance(info)) ||
        CHECK_FAIL(VulkanInit::InitEnumerateDevice(info)) ||
        CHECK_FAIL(VulkanInit::InitSwapchainExtension(info)) ||
        CHECK_FAIL(VulkanInit::InitDevice(info)) ||
        CHECK_FAIL(VulkanInit::InitDebugging(info)) ||
        CHECK_FAIL(VulkanInit::InitSwapChain(info)) ||
        CHECK_FAIL(VulkanInit::InitCommandPool(info)) ||
        CHECK_FAIL(VulkanInit::InitCommandBuffer(info)) ||
        CHECK_FAIL(VulkanInit::InitDeviceQueue(info)) ||
        CHECK_FAIL(VulkanInit::InitDepthBuffer(info)) ||
        CHECK_FAIL(VulkanInit::InitDescriptorAndPipelineLayouts(info)) ||
        CHECK_FAIL(VulkanInit::InitRenderpass(info)) ||
        CHECK_FAIL(VulkanInit::InitDescriptorPool(info)) ||
        CHECK_FAIL(VulkanInit::InitSemaphores(info)) ||
        CHECK_FAIL(VulkanInit::InitFence(info)) ||
        CHECK_FAIL(VulkanInit::InitFramebuffers(info)))
    {
        return false;
    }

    if (!VkShader::InitFramework())
    {
        return false;
    }

    return true;
}

std::string VulkanEngine::CompileShader(int index)
{
    return m_data->shaders[index]->CompileShader();
}

bool VulkanEngine::InitialiseScene(const IScene& scene)
{
    auto& info = *m_data;

    m_data->shaders.push_back(std::unique_ptr<VkShader>(new VkShader(info)));

    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
    // TODO: Make this generic for scene

    auto& info = *m_data;

    for (unsigned int i = 0; i < m_data->shaders.size(); ++i)
    {
        const std::string result = CompileShader(i);
        if (!result.empty())
        {
            Logger::LogError("Vulkan: " + m_data->shaders[i]->GetName() + ": " + result);
            return false;
        }
    }

    if (CHECK_FAIL(InitUniformBuffer()) ||
        CHECK_FAIL(InitDescriptorSet()) ||
        CHECK_FAIL(InitVertexBuffer()) ||
        CHECK_FAIL(InitPipeline()))
    {
        return false;
    }

    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.pNext = NULL;
    cmdBufInfo.flags = 0;
    cmdBufInfo.pInheritanceInfo = NULL;

    for(int i = 0; i < (int)info.cmd.size(); ++i)
    {
        auto& cmd = info.cmd[i];

        if (CHECK_FAIL(vkBeginCommandBuffer(cmd, &cmdBufInfo)))
        {
            return false;
        }

        VkRenderPassBeginInfo rpBegin;
        rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.pNext = NULL;
        rpBegin.renderPass = info.renderPass;
        rpBegin.framebuffer = info.framebuffers[i];
        rpBegin.renderArea.offset.x = 0;
        rpBegin.renderArea.offset.y = 0;
        rpBegin.renderArea.extent.width = WINDOW_WIDTH;
        rpBegin.renderArea.extent.height = WINDOW_HEIGHT;
        rpBegin.clearValueCount = 2;
        rpBegin.pClearValues = info.clearValues;
        vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport;
        viewport.height = (float)WINDOW_HEIGHT;
        viewport.width = (float)WINDOW_WIDTH;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        vkCmdSetViewport(cmd, 0, VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS, &viewport);

        VkRect2D scissor;
        scissor.extent.width = WINDOW_WIDTH;
        scissor.extent.height = WINDOW_HEIGHT;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(cmd, 0, VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS, &scissor);

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &info.descSet, 0, NULL);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);

        VkDeviceSize offsets[1] = { 0 };
        vkCmdBindVertexBuffers(cmd, 0, 1, &info.vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(cmd, info.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmd, info.indexBuffer.count, 1, 0, 0, 1);

        vkCmdEndRenderPass(cmd);

        if (CHECK_FAIL(vkEndCommandBuffer(cmd)))
        {
            return false;
        }
    }

    Logger::LogInfo("Vulkan: Re-Initialised");
    return true;
}

VkResult VulkanEngine::InitUniformBuffer()
{
    auto& info = *m_data;

    VkMemoryRequirements memReqs;
    VkBufferCreateInfo bufferInfo = {};
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = 0;
    allocInfo.memoryTypeIndex = 0;

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(info.uniformBuffer);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VkResult res = vkCreateBuffer(info.device, &bufferInfo, nullptr, &info.uniformData.buffer);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    vkGetBufferMemoryRequirements(info.device, info.uniformData.buffer, &memReqs);
    allocInfo.allocationSize = memReqs.size;
    res = VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                                &allocInfo.memoryTypeIndex);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    res = vkAllocateMemory(info.device, &allocInfo, nullptr, &info.uniformData.memory);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    res = vkBindBufferMemory(info.device, info.uniformData.buffer, info.uniformData.memory, 0);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    info.uniformData.bufferInfo.buffer = info.uniformData.buffer;
    info.uniformData.bufferInfo.offset = 0;
    info.uniformData.bufferInfo.range = sizeof(info.uniformBuffer);

    // Update uniform data
    uint8_t* pData = nullptr;
    res = vkMapMemory(info.device, info.uniformData.memory, 0, sizeof(info.uniformBuffer), 0, (void **)&pData);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    memcpy(pData, &info.uniformBuffer, sizeof(info.uniformBuffer));
    vkUnmapMemory(info.device, info.uniformData.memory);

    return VK_SUCCESS;
}

VkResult VulkanEngine::InitPipeline()
{
    auto& info = *m_data;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    rasterizationState.lineWidth = 1.0f;

    VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
    blendAttachmentState[0].colorWriteMask = 0xf;
    blendAttachmentState[0].blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = blendAttachmentState;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;
    viewportState.scissorCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;

    std::vector<VkDynamicState> dynamicStateEnables;
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
    depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilState.stencilTestEnable = VK_FALSE;
    depthStencilState.front = depthStencilState.back;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VulkanUtils::NUM_SAMPLES;
    multisampleState.pSampleMask = nullptr;

    VkVertexInputBindingDescription vertexInputBinding = {};
    vertexInputBinding.binding = 0;
    vertexInputBinding.stride = sizeof(Vertex);
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 2> vertexInputAttributs;
    vertexInputAttributs[0].binding = 0;
    vertexInputAttributs[0].location = 0;
    vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[0].offset = offsetof(Vertex, position);
    vertexInputAttributs[1].binding = 0;
    vertexInputAttributs[1].location = 1;
    vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributs[1].offset = offsetof(Vertex, color);

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
    vertexInputState.vertexAttributeDescriptionCount = 2;
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = info.pipelineLayout;
    pipelineCreateInfo.renderPass = info.renderPass;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(info.shaderStages.size());
    pipelineCreateInfo.pStages = info.shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.renderPass = info.renderPass;
    pipelineCreateInfo.pDynamicState = &dynamicState;

    VkResult res = vkCreateGraphicsPipelines(info.device, info.pipelineCache, 1, &pipelineCreateInfo, nullptr, &info.pipeline);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.pipeline, typeid(info.pipeline), "Pipeline");
    return VK_SUCCESS;
}

VkResult VulkanEngine::InitVertexBuffer()
{
    auto& info = *m_data;
    
    static std::vector<Vertex> vertexBuffer =
    {
        { { 1.0f,  1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        { { -1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.0f, -1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
    };
    uint32_t vertexBufferSize = static_cast<uint32_t>(vertexBuffer.size()) * sizeof(Vertex);
    
    std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
    info.indexBuffer.count = static_cast<uint32_t>(indexBuffer.size());
    uint32_t indexBufferSize = info.indexBuffer.count * sizeof(uint32_t);
    
    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs;
    void* data = nullptr;
    
    VkBufferCreateInfo vertexBufferInfo = {};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.size = vertexBufferSize;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    // Copy vertex data to a buffer visible to the host
    VkResult res = vkCreateBuffer(info.device, &vertexBufferInfo, nullptr, &info.vertexBuffer.buffer);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    vkGetBufferMemoryRequirements(info.device, info.vertexBuffer.buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    
    res = VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
                                                &memAlloc.memoryTypeIndex);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    res = vkAllocateMemory(info.device, &memAlloc, nullptr, &info.vertexBuffer.memory);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    res = vkMapMemory(info.device, info.vertexBuffer.memory, 0, memAlloc.allocationSize, 0, &data);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    memcpy(data, vertexBuffer.data(), vertexBufferSize);
    vkUnmapMemory(info.device, info.vertexBuffer.memory);
    res = vkBindBufferMemory(info.device, info.vertexBuffer.buffer, info.vertexBuffer.memory, 0);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    VkBufferCreateInfo indexbufferInfo = {};
    indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexbufferInfo.size = indexBufferSize;
    indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    
    // Copy index data to a buffer visible to the host
    res = vkCreateBuffer(info.device, &indexbufferInfo, nullptr, &info.indexBuffer.buffer);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    vkGetBufferMemoryRequirements(info.device, info.indexBuffer.buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    
    res = VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
                                                &memAlloc.memoryTypeIndex);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    res = vkAllocateMemory(info.device, &memAlloc, nullptr, &info.indexBuffer.memory);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    res = vkMapMemory(info.device, info.indexBuffer.memory, 0, indexBufferSize, 0, &data);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    memcpy(data, indexBuffer.data(), indexBufferSize);
    vkUnmapMemory(info.device, info.indexBuffer.memory);
    res = vkBindBufferMemory(info.device, info.indexBuffer.buffer, info.indexBuffer.memory, 0);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }
    
    return VK_SUCCESS;
}

VkResult VulkanEngine::InitDescriptorSet()
{
    auto& info = *m_data;

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = info.descPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &info.descLayout;

    VkResult res = vkAllocateDescriptorSets(info.device, &allocInfo, &info.descSet);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = info.descSet;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.pBufferInfo = &info.uniformData.bufferInfo;
    writeDescriptorSet.dstBinding = 0;

    vkUpdateDescriptorSets(info.device, 1, &writeDescriptorSet, 0, nullptr);

    return VK_SUCCESS;
}

bool VulkanEngine::FadeView(bool in, float amount)
{
    return true;
}

void VulkanEngine::Render(const IScene& scene, float timer)
{
    VulkanUtils::Render(*m_data);
}

void VulkanEngine::ToggleWireframe()
{

}

std::string VulkanEngine::GetName() const
{
    return "Vulkan";
}

void VulkanEngine::UpdateView(const Matrix& world)
{
    //glm::mat4 view;
    //
    //view[0][0] = world.m11;
    //view[1][0] = world.m12;
    //view[2][0] = world.m13;
    //view[3][0] = world.m14;
    //
    //view[0][1] = world.m21;
    //view[1][1] = world.m22;
    //view[2][1] = world.m23;
    //view[3][1] = world.m24;
    //
    //view[0][2] = world.m31;
    //view[1][2] = world.m32;
    //view[2][2] = world.m33;
    //view[3][2] = world.m34;
    //
    //m_data->cameraPosition.x = world.m14;
    //m_data->cameraPosition.y = world.m24;
    //m_data->cameraPosition.z = world.m34;
    //
    //m_data->cameraUp.x = world.m12;
    //m_data->cameraUp.y = world.m22;
    //m_data->cameraUp.z = world.m32;
    //
    //m_data->view = glm::inverse(view);
    //m_data->viewProjection = m_data->projection * m_data->view;
}

std::string VulkanEngine::GetShaderText(int index) const
{
    return "";
}

std::string VulkanEngine::GetShaderAssembly(int index)
{
    return "";
}

void VulkanEngine::SetFade(float value)
{
    
}

void VulkanEngine::ReloadTerrain(int index)
{
}

void VulkanEngine::ReloadTexture(int index)
{
}

void VulkanEngine::WriteToShader(const Shader& shader,
                                 const std::string& text)
{
}