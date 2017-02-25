////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
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

    if (FAILED(VulkanUtils::InitGlobalLayerProperties(info)) ||
        FAILED(VulkanUtils::InitExtensionNames(info)) ||
        FAILED(VulkanUtils::InitInstance(info)) ||
        FAILED(VulkanUtils::InitDebugging(info)) ||
        FAILED(VulkanUtils::InitEnumerateDevice(info)) ||
        FAILED(VulkanUtils::InitSwapchainExtension(info)) ||
        FAILED(VulkanUtils::InitDevice(info)) ||
        FAILED(VulkanUtils::InitCommandPool(info)) ||
        FAILED(VulkanUtils::InitCommandBuffer(info)) ||
        FAILED(VulkanUtils::InitViewports(info)) ||
        FAILED(VulkanUtils::InitDeviceQueue(info)) ||
        FAILED(VulkanUtils::InitSwapChain(info)) ||
        FAILED(VulkanUtils::InitDepthBuffer(info)) ||
        FAILED(VulkanUtils::InitUniformBuffer(info)) ||
        FAILED(VulkanUtils::InitDescriptorAndPipelineLayouts(info)) ||
        FAILED(VulkanUtils::InitRenderpass(info)) ||
        FAILED(VulkanUtils::InitDescriptorPool(info)) ||
        FAILED(VulkanUtils::InitDescriptorSet(info)) ||
        FAILED(VulkanUtils::InitScissors(info)) ||
        FAILED(VulkanUtils::InitSemaphores(info)) ||
        FAILED(VulkanUtils::InitFence(info)))
    {
        return false;
    }

    if (!VkShader::InitFramework())
    {
        return false;
    }

    VulkanUtils::BeginCommandBuffer(info);

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

    if (FAILED(VulkanUtils::InitFramebuffers(info)) ||
        FAILED(VulkanUtils::InitVertexBuffer(info)) ||
        FAILED(VulkanUtils::InitPipeline(info)))
    {
        return false;
    }

    Logger::LogInfo("Vulkan: Re-Initialised");
    return true;
}

bool VulkanEngine::FadeView(bool in, float amount)
{
    return true;
}

void VulkanEngine::Render(const IScene& scene, float timer)
{
    auto& info = *m_data;

    FAILED(vkAcquireNextImageKHR(info.device, 
                                 info.swapChain, 
                                 UINT64_MAX, 
                                 info.imageAcquiredSemaphore, 
                                 VK_NULL_HANDLE,
                                 &info.currentBuffer));

    VkRenderPassBeginInfo rpBegin;
    rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBegin.pNext = NULL;
    rpBegin.renderPass = info.renderPass;
    rpBegin.framebuffer = info.framebuffers[info.currentBuffer];
    rpBegin.renderArea.offset.x = 0;
    rpBegin.renderArea.offset.y = 0;
    rpBegin.renderArea.extent.width = WINDOW_WIDTH;
    rpBegin.renderArea.extent.height = WINDOW_HEIGHT;
    rpBegin.clearValueCount = 2;
    rpBegin.pClearValues = &info.clearValues[0];

    vkCmdBeginRenderPass(info.cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);

    vkCmdBindDescriptorSets(info.cmd, 
                            VK_PIPELINE_BIND_POINT_GRAPHICS, 
                            info.pipelineLayout, 
                            0, 
                            VulkanUtils::NUM_DESCRIPTOR_SETS, 
                            info.descSet.data(), 
                            0, 
                            NULL);

    const VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(info.cmd, 0, 1, &info.vertexBuffer.buffer, offsets);

    vkCmdDraw(info.cmd, 12 * 3, 1, 0, 0);
    vkCmdEndRenderPass(info.cmd);
    VulkanUtils::EndCommandBuffer(info);

    const VkCommandBuffer cmd_bufs[] = { info.cmd };
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo[1] = {};
    submitInfo[0].pNext = NULL;
    submitInfo[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo[0].waitSemaphoreCount = 1;
    submitInfo[0].pWaitSemaphores = &info.imageAcquiredSemaphore;
    submitInfo[0].pWaitDstStageMask = &pipe_stage_flags;
    submitInfo[0].commandBufferCount = 1;
    submitInfo[0].pCommandBuffers = cmd_bufs;
    submitInfo[0].signalSemaphoreCount = 0;
    submitInfo[0].pSignalSemaphores = NULL;

    // Queue the command buffer for execution
    FAILED(vkQueueSubmit(info.graphicsQueue, 1, submitInfo, info.drawFence));

    // Now present the image in the window
    VkPresentInfoKHR present;
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &info.swapChain;
    present.pImageIndices = &info.currentBuffer;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;

    // Make sure command buffer is finished before presenting
    VkResult res = VK_TIMEOUT;
    while (res == VK_TIMEOUT)
    {
        res = vkWaitForFences(info.device, 1, &info.drawFence, VK_TRUE, VulkanUtils::FENCE_TIMEOUT);
    }

    FAILED(vkQueuePresentKHR(info.presentQueue, &present));

    VulkanUtils::BeginCommandBuffer(info);
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