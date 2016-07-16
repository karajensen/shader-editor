////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulcanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulcanengine.h"
#include "sceneInterface.h"
#include <array>
#include <fstream>

#include "vulkan\include\vulkan.h"

/**
* Internal data for the vulcan rendering engine
*/
struct VulcanData
{
    /**
    * Constructor
    */
    VulcanData();

    /**
    * Destructor
    */
    ~VulcanData();

    /**
    * Releases the device/context
    */
    void Release();
};

VulcanData::VulcanData()
{
}

VulcanData::~VulcanData()
{
    Release();
}

void VulcanData::Release()
{
}

VulcanEngine::VulcanEngine(HWND hwnd) :
    m_data(new VulcanData()),
    m_hwnd(hwnd)
{
}

VulcanEngine::~VulcanEngine()
{
    Release();
}

void VulcanEngine::Release()
{
    m_data->Release();
}

bool VulcanEngine::Initialize()
{
    Logger::LogInfo("Vulcan: Initialize sucessful");
    return true;
}

std::string VulcanEngine::CompileShader(int index)
{
    return "";
}

bool VulcanEngine::InitialiseScene(const IScene& scene)
{
    return ReInitialiseScene();
}

bool VulcanEngine::ReInitialiseScene()
{
    Logger::LogInfo("Vulcan: Re-Initialised");
    return true;
}

bool VulcanEngine::FadeView(bool in, float amount)
{
    return true;
}

void VulcanEngine::Render(const IScene& scene, float timer)
{

}

void VulcanEngine::ToggleWireframe()
{

}

std::string VulcanEngine::GetName() const
{
    return "Vulcan";
}

void VulcanEngine::UpdateView(const Matrix& world)
{
}

std::string VulcanEngine::GetShaderText(int index) const
{
    return "";
}

std::string VulcanEngine::GetShaderAssembly(int index)
{
    return "";
}

void VulcanEngine::SetFade(float value)
{
    
}

void VulcanEngine::ReloadTerrain(int index)
{
}

void VulcanEngine::ReloadTexture(int index)
{
}

void VulcanEngine::WriteToShader(const Shader& shader,
                                 const std::string& text)
{
}