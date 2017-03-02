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

    if (!VulkanInit::InitGlobalLayerProperties(info) ||
        !VulkanInit::InitInstance(info) ||
        !VulkanInit::InitEnumerateDevice(info) ||
        !VulkanInit::InitSwapchainExtension(info) ||
        !VulkanInit::InitDevice(info) ||
        !VulkanInit::InitDebugging(info) ||
        !VulkanInit::InitSwapChain(info) ||
        !VulkanInit::InitCommandPool(info) ||
        !VulkanInit::InitCommandBuffer(info) ||
        !VulkanInit::InitDeviceQueue(info) ||
        !VulkanInit::InitDepthBuffer(info) ||
        !VulkanInit::InitDescriptorAndPipelineLayouts(info) ||
        !VulkanInit::InitRenderpass(info) ||
        !VulkanInit::InitDescriptorPool(info) ||
        !VulkanInit::InitSemaphores(info) ||
        !VulkanInit::InitFence(info) ||
        !VulkanInit::InitFramebuffers(info) ||
        !VulkanInit::InitDescriptorSet(info))
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

    m_data->shaders.reserve(scene.Shaders().size());
    for (const auto& shader : scene.Shaders())
    {
        m_data->shaders.push_back(std::unique_ptr<VkShader>(
            new VkShader(info, *shader)));
    }

    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
    // TODO: Make this generic for scene
    auto& info = *m_data;

    for (unsigned int i = 0; i < m_data->shaders.size(); ++i)
    {
        const std::string result = m_data->shaders[i]->Initialise();
        if (!result.empty())
        {
            Logger::LogError("Vulkan: " + m_data->shaders[i]->GetName() + ": " + result);
            return false;
        }
    }

    if (!InitVertexBuffer())
    {
        return false;
    }

    SetSelectedShader(0);

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

        info.shaders.at(info.selectedShader)->Bind(cmd);

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

bool VulkanEngine::InitVertexBuffer()
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
    if (CHECK_FAIL(vkCreateBuffer(info.device, &vertexBufferInfo, nullptr, &info.vertexBuffer.buffer)))
    {
        return false;
    }
    
    vkGetBufferMemoryRequirements(info.device, info.vertexBuffer.buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    
    if (CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
                                                         &memAlloc.memoryTypeIndex)))
    {
        return false;
    }
    
    if (CHECK_FAIL(vkAllocateMemory(info.device, &memAlloc, nullptr, &info.vertexBuffer.memory)))
    {
        return false;
    }
    
    if (CHECK_FAIL(vkMapMemory(info.device, info.vertexBuffer.memory, 0, memAlloc.allocationSize, 0, &data)))
    {
        return false;
    }
    
    memcpy(data, vertexBuffer.data(), vertexBufferSize);
    vkUnmapMemory(info.device, info.vertexBuffer.memory);
    if (CHECK_FAIL(vkBindBufferMemory(info.device, info.vertexBuffer.buffer, info.vertexBuffer.memory, 0)))
    {
        return false;
    }
    
    VkBufferCreateInfo indexbufferInfo = {};
    indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexbufferInfo.size = indexBufferSize;
    indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    
    // Copy index data to a buffer visible to the host
    if (CHECK_FAIL(vkCreateBuffer(info.device, &indexbufferInfo, nullptr, &info.indexBuffer.buffer)))
    {
        return false;
    }
    
    vkGetBufferMemoryRequirements(info.device, info.indexBuffer.buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    
    if (CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
                                                         &memAlloc.memoryTypeIndex)))
    {
        return false;
    }
    
    if (CHECK_FAIL(vkAllocateMemory(info.device, &memAlloc, nullptr, &info.indexBuffer.memory)))
    {
        return false;
    }
    
    if (CHECK_FAIL(vkMapMemory(info.device, info.indexBuffer.memory, 0, indexBufferSize, 0, &data)))
    {
        return false;
    }
    
    memcpy(data, indexBuffer.data(), indexBufferSize);
    vkUnmapMemory(info.device, info.indexBuffer.memory);
    if (CHECK_FAIL(vkBindBufferMemory(info.device, info.indexBuffer.buffer, info.indexBuffer.memory, 0)))
    {
        return false;
    }
    
    return true;
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

void VulkanEngine::SetSelectedShader(int index)
{
    m_data->selectedShader = index;
    m_data->shaders[index]->SetActive();
}
