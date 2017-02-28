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

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0,
            VulkanUtils::NUM_DESCRIPTOR_SETS, info.descSet.data(), 0, NULL);

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

    float fov = glm::radians(45.0f);
    if (WINDOW_WIDTH > WINDOW_HEIGHT)
    {
        fov *= static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(WINDOW_WIDTH);
    }

    info.projection = glm::perspective(fov, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);

    info.view =
        glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
            glm::vec3(0, 0, 0),                 // and looks at the origin
            glm::vec3(0, -1, 0)                 // Head is up (set to 0,-1,0 to look upside-down)
        );
    info.model = glm::mat4(1.0f);

    // Vulkan clip space has inverted Y and half Z.
    info.clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

    info.mvp = info.clip * info.projection * info.view * info.model;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = NULL;
    bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.size = sizeof(info.mvp);
    bufInfo.queueFamilyIndexCount = 0;
    bufInfo.pQueueFamilyIndices = NULL;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags = 0;
    VkResult res = vkCreateBuffer(info.device, &bufInfo, NULL, &info.uniformData.buffer);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.uniformData.buffer,
        typeid(info.uniformData.buffer), "UniformBuffer");

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(info.device, info.uniformData.buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.memoryTypeIndex = 0;

    allocInfo.allocationSize = memReqs.size;
    res = VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                                &allocInfo.memoryTypeIndex);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    res = vkAllocateMemory(info.device, &allocInfo, NULL, &(info.uniformData.memory));
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.uniformData.memory,
        typeid(info.uniformData.memory), "UniformMemory");

    uint8_t *pData;
    res = vkMapMemory(info.device, info.uniformData.memory, 0, memReqs.size, 0, (void **)&pData);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    memcpy(pData, &info.mvp, sizeof(info.mvp));

    vkUnmapMemory(info.device, info.uniformData.memory);

    res = vkBindBufferMemory(info.device, info.uniformData.buffer, info.uniformData.memory, 0);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    info.uniformData.bufferInfo.buffer = info.uniformData.buffer;
    info.uniformData.bufferInfo.offset = 0;
    info.uniformData.bufferInfo.range = sizeof(info.mvp);
    return VK_SUCCESS;
}

VkResult VulkanEngine::InitPipeline()
{
    auto& info = *m_data;

    VkPipelineCacheCreateInfo pipelineCache;
    pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCache.pNext = NULL;
    pipelineCache.initialDataSize = 0;
    pipelineCache.pInitialData = NULL;
    pipelineCache.flags = 0;
    VkResult res = vkCreatePipelineCache(info.device, &pipelineCache, NULL, &info.pipelineCache);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkBool32 includeDepth = true;
    VkBool32 include_vi = true;

    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;

    VkPipelineVertexInputStateCreateInfo vi;
    memset(&vi, 0, sizeof(vi));
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (include_vi)
    {
        vi.pNext = NULL;
        vi.flags = 0;
        vi.vertexBindingDescriptionCount = 1;
        vi.pVertexBindingDescriptions = &info.viBinding;
        vi.vertexAttributeDescriptionCount = info.viAttribs.size();
        vi.pVertexAttributeDescriptions = &info.viAttribs[0];
    }
    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rs;
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rs.depthClampEnable = includeDepth;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.depthBiasConstantFactor = 0;
    rs.depthBiasClamp = 0;
    rs.depthBiasSlopeFactor = 0;
    rs.lineWidth = 1.0f;

    VkPipelineColorBlendStateCreateInfo cb;
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.flags = 0;
    cb.pNext = NULL;

    VkPipelineColorBlendAttachmentState attState[1];
    attState[0].colorWriteMask = 0xf;
    attState[0].blendEnable = VK_FALSE;
    attState[0].alphaBlendOp = VK_BLEND_OP_ADD;
    attState[0].colorBlendOp = VK_BLEND_OP_ADD;
    attState[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    cb.attachmentCount = 1;
    cb.pAttachments = attState;
    cb.logicOpEnable = VK_FALSE;
    cb.logicOp = VK_LOGIC_OP_NO_OP;
    cb.blendConstants[0] = 1.0f;
    cb.blendConstants[1] = 1.0f;
    cb.blendConstants[2] = 1.0f;
    cb.blendConstants[3] = 1.0f;

    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = VulkanUtils::NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
    vp.pScissors = NULL;
    vp.pViewports = NULL;

    VkPipelineDepthStencilStateCreateInfo ds;
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;
    ds.flags = 0;
    ds.depthTestEnable = includeDepth;
    ds.depthWriteEnable = includeDepth;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.stencilTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.back.compareMask = 0;
    ds.back.reference = 0;
    ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
    ds.back.writeMask = 0;
    ds.minDepthBounds = 0;
    ds.maxDepthBounds = 0;
    ds.stencilTestEnable = VK_FALSE;
    ds.front = ds.back;

    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = VulkanUtils::NUM_SAMPLES;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;

    VkGraphicsPipelineCreateInfo pipeline;
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.pNext = NULL;
    pipeline.layout = info.pipelineLayout;
    pipeline.basePipelineHandle = VK_NULL_HANDLE;
    pipeline.basePipelineIndex = 0;
    pipeline.flags = 0;
    pipeline.pVertexInputState = &vi;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &rs;
    pipeline.pColorBlendState = &cb;
    pipeline.pTessellationState = NULL;
    pipeline.pMultisampleState = &ms;
    pipeline.pDynamicState = &dynamicState;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &ds;
    pipeline.pStages = &info.shaderStages[0];
    pipeline.stageCount = info.shaderStages.size();
    pipeline.renderPass = info.renderPass;
    pipeline.subpass = 0;

    res = vkCreateGraphicsPipelines(info.device, info.pipelineCache, 1, &pipeline, NULL, &info.pipeline);
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

    struct Vertex 
    {
        float position[3];
        float color[3];
    };

    std::vector<Vertex> vertexBuffer =
    {
        { { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
        { { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
        { { 0.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
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

    bool use_texture = false;

    VkDescriptorSetAllocateInfo allocInfo[1];
    allocInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo[0].pNext = NULL;
    allocInfo[0].descriptorPool = info.descPool;
    allocInfo[0].descriptorSetCount = VulkanUtils::NUM_DESCRIPTOR_SETS;
    allocInfo[0].pSetLayouts = info.descLayout.data();

    info.descSet.resize(VulkanUtils::NUM_DESCRIPTOR_SETS);
    VkResult res = vkAllocateDescriptorSets(info.device, allocInfo, info.descSet.data());
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    for (int i = 0; i < (int)info.descSet.size(); ++i)
    {
        VulkanUtils::SetDebugName(info, (uint64_t)info.descSet[i], typeid(info.descSet[i]),
            ("DescSet" + std::to_string(i)).c_str());
    }

    VkWriteDescriptorSet writes[2];

    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].dstSet = info.descSet[0];
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo = &info.uniformData.bufferInfo;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;

    if (use_texture)
    {
        writes[1] = {};
        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = info.descSet[0];
        writes[1].dstBinding = 1;
        writes[1].descriptorCount = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[1].pImageInfo = &info.imageInfo;
        writes[1].dstArrayElement = 0;
    }

    vkUpdateDescriptorSets(info.device, use_texture ? 2 : 1, writes, 0, NULL);
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