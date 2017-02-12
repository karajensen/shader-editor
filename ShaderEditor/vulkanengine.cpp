////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkandata.h"
#include "vulkanutils.h"
#include "sceneInterface.h"
#include <array>
#include <fstream>
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"

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
    init_global_layer_properties(*m_data);

    // initialize the VkApplicationInfo structure
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = "";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    // initialize the VkInstanceCreateInfo structure
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = nullptr;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = nullptr;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = nullptr;

    VkInstance instance;
    if (FAIL(vkCreateInstance(&inst_info, nullptr, &instance)))
    {
        return false;
    }

    vkDestroyInstance(instance, nullptr);
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