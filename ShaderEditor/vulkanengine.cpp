////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkandata.h"
#include "vulkanutils.h"
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

    if (FAILED(VulkanUtils::init_global_layer_properties(info)) ||
        FAILED(VulkanUtils::init_instance_extension_names(info)) ||
        FAILED(VulkanUtils::init_device_extension_names(info)) ||
        FAILED(VulkanUtils::init_instance(info)) ||
        FAILED(VulkanUtils::init_debugging(info)) ||
        FAILED(VulkanUtils::init_enumerate_device(info)) ||
        FAILED(VulkanUtils::init_swapchain_extension(info)) ||
        FAILED(VulkanUtils::init_device(info)) ||
        FAILED(VulkanUtils::init_command_pool(info)) ||
        FAILED(VulkanUtils::init_command_buffer(info)) ||
        FAILED(VulkanUtils::init_device_queue(info)) ||
        FAILED(VulkanUtils::init_swap_chain(info)) ||
        FAILED(VulkanUtils::init_depth_buffer(info)) ||
        FAILED(VulkanUtils::init_uniform_buffer(info)) ||
        FAILED(VulkanUtils::init_descriptor_and_pipeline_layouts(info)) ||
        FAILED(VulkanUtils::init_renderpass(info)) ||
        FAILED(VulkanUtils::init_shaders(info)) ||
        FAILED(VulkanUtils::init_framebuffers(info)) ||
        FAILED(VulkanUtils::init_vertex_buffer(info)) ||
        FAILED(VulkanUtils::init_descriptor_pool(info)) ||
        FAILED(VulkanUtils::init_descriptor_set(info)) ||
        FAILED(VulkanUtils::init_pipeline_cache(info)) ||
        FAILED(VulkanUtils::init_pipeline(info)) ||
        FAILED(VulkanUtils::init_viewports(info)) ||
        FAILED(VulkanUtils::init_scissors(info)) ||
        FAILED(VulkanUtils::init_semaphores(info)) ||
        FAILED(VulkanUtils::init_fence(info)))
    {
        return false;
    }

    //TODO: Scene initialisation

    VulkanUtils::begin_command_buffer(info);
    return true;
}

std::string VulkanEngine::CompileShader(int index)
{
    return "";
}

bool VulkanEngine::InitialiseScene(const IScene& scene)
{
    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
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
                                 info.swap_chain, 
                                 UINT64_MAX, 
                                 info.imageAcquiredSemaphore, 
                                 VK_NULL_HANDLE,
                                 &info.current_buffer));

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = info.render_pass;
    rp_begin.framebuffer = info.framebuffers[info.current_buffer];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = WINDOW_WIDTH;
    rp_begin.renderArea.extent.height = WINDOW_HEIGHT;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = &info.clear_values[0];

    vkCmdBeginRenderPass(info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);

    vkCmdBindDescriptorSets(info.cmd, 
                            VK_PIPELINE_BIND_POINT_GRAPHICS, 
                            info.pipeline_layout, 
                            0, 
                            VulkanUtils::NUM_DESCRIPTOR_SETS, 
                            info.desc_set.data(), 
                            0, 
                            NULL);

    const VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(info.cmd, 0, 1, &info.vertex_buffer.buffer, offsets);

    vkCmdDraw(info.cmd, 12 * 3, 1, 0, 0);
    vkCmdEndRenderPass(info.cmd);
    VulkanUtils::end_command_buffer(info);

    const VkCommandBuffer cmd_bufs[] = { info.cmd };
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores = &info.imageAcquiredSemaphore;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    // Queue the command buffer for execution
    FAILED(vkQueueSubmit(info.graphics_queue, 1, submit_info, info.drawFence));

    // Now present the image in the window
    VkPresentInfoKHR present;
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &info.swap_chain;
    present.pImageIndices = &info.current_buffer;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;

    // Make sure command buffer is finished before presenting
    VkResult res = VK_TIMEOUT;
    while (res == VK_TIMEOUT)
    {
        res = vkWaitForFences(info.device, 1, &info.drawFence, VK_TRUE, VulkanUtils::FENCE_TIMEOUT);
    }

    FAILED(vkQueuePresentKHR(info.present_queue, &present));

    VulkanUtils::begin_command_buffer(info);
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