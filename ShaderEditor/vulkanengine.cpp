////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkaninit.h"
#include "vulkandata.h"
#include "vulkanmesh.h"
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

    m_data->meshes.reserve(scene.Meshes().size());
    for (const auto& mesh : scene.Meshes())
    {
        m_data->meshes.push_back(std::unique_ptr<VkMesh>(
            new VkMesh(info, *mesh)));
    }

    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
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

    for (auto& mesh : m_data->meshes)
    {
        if (!mesh->Initialise())
        {
            Logger::LogError("Vulkan: Failed to re-initialise mesh");
            return false;
        }
    }

    SetSelectedShader(0);

    // TO DO: Add to render loop
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

        for (auto& mesh : m_data->meshes)
        {
            mesh->Bind(cmd);
        }

        vkCmdEndRenderPass(cmd);

        if (CHECK_FAIL(vkEndCommandBuffer(cmd)))
        {
            return false;
        }
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

    info.shaders.at(info.selectedShader)->UpdateUniformBuffer();
    VulkanUtils::Render(info);
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
    glm::mat4 view;
    
    view[0][0] = world.m11;
    view[1][0] = world.m12;
    view[2][0] = world.m13;
    view[3][0] = world.m14;
    
    view[0][1] = world.m21;
    view[1][1] = world.m22;
    view[2][1] = world.m23;
    view[3][1] = world.m24;
    
    view[0][2] = world.m31;
    view[1][2] = world.m32;
    view[2][2] = world.m33;
    view[3][2] = world.m34;
    
    m_data->cameraPosition.x = world.m14;
    m_data->cameraPosition.y = world.m24;
    m_data->cameraPosition.z = world.m34;
    
    m_data->cameraUp.x = world.m12;
    m_data->cameraUp.y = world.m22;
    m_data->cameraUp.z = world.m32;
    
    m_data->view = glm::inverse(view);
    m_data->viewProjection = m_data->projection * m_data->view;
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
