////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "timer.h"
#include "openglengine.h"
#include "directxengine.h"
#include "scene.h"
#include "camera.h"
#include "cache.h"
#include <windowsx.h>
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/regex.hpp"

//#define SELECTED_ENGINE DIRECTX
#define SELECTED_ENGINE OPENGL

namespace
{
    const float CAMERA_MOVE_SPEED = 50.0f; ///< Speed the camera will translate
    const float CAMERA_ROT_SPEED = 2.0f;   ///< Speed the camera will rotate
    const float CAMERA_SIDE_SPEED = 30.0f; ///< Speed the camera will strafe
    const float FADE_AMOUNT = 0.02f;       ///< Speed to fade the engine in/out
}

Application::Application(std::shared_ptr<Cache> cache) :
    m_camera(new Camera()),
    m_cache(cache),
    m_selectedPost(Texture::SCENE_TEXTURE),
    m_selectedEngine(SELECTED_ENGINE)
{
}

Application::~Application()
{
}

void Application::Run()
{
    WPARAM keyDown;
    MSG msg;
    m_timer->StartTimer();
    bool runApplication = true;
    
    while(runApplication)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if(IsKeyDown(VK_ESCAPE) || msg.message == WM_QUIT)
            {
                runApplication = false;
                m_cache->ApplicationRunning.Set(false);
            }
            HandleInputEvents(keyDown, msg);
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
        else
        {
            m_timer->UpdateTimer();
            TickApplication();
        }
    }
}

bool Application::IsKeyDown(unsigned int key) const
{
    return (GetAsyncKeyState(key) & 0x8000) ? true : false;
}

void Application::HandleKeyPress(const WPARAM& keypress)
{
    if(keypress == VK_F2)
    {
        int index = m_selectedEngine + 1;
        if(index >= static_cast<int>(m_engines.size()))
        {
            index = 0;
        }

        m_cache->EngineSelected.SetUpdated(index);
        SwitchRenderEngine(index);
        m_engines[index]->SetFade(1.0f);
    }
    else if(keypress == VK_F3)
    {
        m_scene->SaveLightsToFile();
    }
    else if(keypress == VK_F4)
    {
        m_scene->SaveMeshesToFile();
    }
}

void Application::HandleInputEvents(WPARAM& keydown, const MSG& msg)
{
    switch(msg.message)
    {
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        keydown = msg.wParam;
        break;
    case WM_SYSKEYUP:
    case WM_KEYUP:
        HandleKeyPress(keydown);
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        m_mousePressed = false;
        break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        m_mousePressed = true;
        break;
    case WM_MOUSEMOVE:
        HandleMouseMovement(msg);
        break;
    }
}

void Application::HandleMouseMovement(const MSG& msg)
{
    const int ix = GET_X_LPARAM(msg.lParam);
    const int iy = GET_Y_LPARAM(msg.lParam);
    const float x = static_cast<float>(ix);
    const float y = static_cast<float>(iy);

    // Determine the direction the mouse is moving
    m_mouseDirection.x = m_mousePosition.x - x;
    m_mouseDirection.y = m_mousePosition.y - y;

    const float length = std::sqrt(
        (m_mouseDirection.x*m_mouseDirection.x) + 
        (m_mouseDirection.y*m_mouseDirection.y));

    if(length != 0.0f)
    {
        m_mouseDirection.x /= length;
        m_mouseDirection.y /= length;
    }

    // Adjust camera according to the movement
    if(IsKeyDown(VK_MENU))
    {
        m_camera->Rotation(m_mouseDirection, m_mousePressed,
            m_timer->GetDeltaTime()*CAMERA_ROT_SPEED);
    }
    else if(IsKeyDown(VK_SHIFT))
    {
        m_camera->ForwardMovement(m_mouseDirection, m_mousePressed,
            m_timer->GetDeltaTime()*CAMERA_MOVE_SPEED);
    }
    else if(IsKeyDown(VK_CONTROL))
    {
        m_camera->SideMovement(m_mouseDirection, m_mousePressed,
            m_timer->GetDeltaTime()*CAMERA_SIDE_SPEED);
    }

    m_mousePosition.x = x;
    m_mousePosition.y = y;
}

void Application::TickApplication()
{
    if(m_camera->RequiresUpdate())
    {
        m_camera->Update();
        GetEngine()->UpdateView(m_camera->GetWorld());
    }

    FadeRenderEngine();
    GetEngine()->Render(m_scene->GetLights());

    UpdateShader();
    switch(m_cache->PageSelected.Get())
    {
    case SCENE:
        UpdateScene();
        break;
    case LIGHT:
        UpdateLight();
        break;
    case MESH:
        UpdateMesh();
        break;
    case POST:
        UpdatePost();
        break;
    }

    m_mouseDirection.x = 0;
    m_mouseDirection.y = 0;
}

bool Application::ReCompileShader(const std::string& text)
{
    const std::string& name = m_scene->GetShader(m_selectedShader).name;
    m_cache->CompileShader.Clear();

    if(m_selectedEngine == OPENGL)
    {
        // GLSL uses two files that both must start with GLSL_HEADER
        // Note first component in split regex vector is whitespace or empty
        std::vector<std::string> components;
        boost::algorithm::split_regex(components, text, boost::regex(GLSL_HEADER));
        m_scene->WriteToShader(name, GLSL_HEADER + components[1], GLSL_VERTEX_EXTENSION);
        m_scene->WriteToShader(name, GLSL_HEADER + components[2], GLSL_FRAGMENT_EXTENSION);
    }
    else if(m_selectedEngine == DIRECTX)
    {
        m_scene->WriteToShader(name, text, HLSL_SHADER_EXTENSION);
    }

    std::string errors = GetEngine()->CompileShader(m_selectedShader);
    if(errors.empty())
    {
        Logger::LogInfo(name + ": Recompiled successfully");
        return true;
    }
    else
    {
        Logger::LogInfo(name + ": Failed Recompilation");
        ShowMessage(name + ":" + errors, "Compilation Errors");
        return false;
    }
}

void Application::UpdateShader()
{   
    const int selectedShader = m_cache->ShaderSelected.Get();
    bool changedShader = selectedShader != m_selectedShader;
    bool recompiledShader = false;

    const std::string updatedText = m_cache->CompileShader.Get();
    if(!updatedText.empty() && m_selectedShader != NO_INDEX)
    {
        recompiledShader = ReCompileShader(updatedText);
    }

    if(changedShader || recompiledShader)
    {
        m_selectedShader = selectedShader;
        m_cache->ShaderText.SetUpdated(GetEngine()->GetShaderText(m_selectedShader));
        m_cache->ShaderAsm.SetUpdated(GetEngine()->GetShaderAssembly(m_selectedShader));
    }
}

void Application::UpdateScene()
{
    const int selectedEngine = m_cache->EngineSelected.Get();
    if(selectedEngine != m_selectedEngine)
    {
        m_fadeState = FADE_OUT;
    }

    m_cache->FramesPerSec.Set(m_timer->GetFPS());
    m_cache->DeltaTime.Set(m_timer->GetDeltaTime());
    m_cache->MousePosition.Set(m_mousePosition);
    m_cache->MouseDirection.Set(m_mouseDirection);
    m_cache->CameraPosition.Set(m_camera->GetWorld().Position());
}

void Application::UpdateMesh()
{
    const int selectedMesh = m_cache->MeshSelected.Get();
    if(selectedMesh != m_selectedMesh)
    {
        m_selectedMesh = selectedMesh;

        auto& mesh = m_scene->GetMesh(m_selectedMesh);
        const int diffuse = mesh.textureIDs[Texture::DIFFUSE];
        const int normal = mesh.textureIDs[Texture::NORMAL];
        const int specular = mesh.textureIDs[Texture::SPECULAR];

        m_cache->MeshBackFaceCull.SetUpdated(mesh.backfacecull);
        m_cache->MeshSpecularity.SetUpdated(mesh.specularity);
        m_cache->MeshAmbience.SetUpdated(mesh.ambience);
        m_cache->MeshBump.SetUpdated(mesh.bump);
        m_cache->MeshTransparency.SetUpdated(m_scene->HasTransparency(m_selectedMesh));
        m_cache->MeshShader.SetUpdated(m_scene->GetShader(mesh.shaderIndex).name);
        m_cache->MeshDiffuse.SetUpdated(m_scene->GetTexture(diffuse));
        m_cache->MeshNormal.SetUpdated(m_scene->GetTexture(normal));        
        m_cache->MeshSpecular.SetUpdated(m_scene->GetTexture(specular));
    }
    else if(m_selectedMesh >= 0 && m_selectedMesh < m_scene->GetMeshCount())
    {
        auto& mesh = m_scene->GetMesh(m_selectedMesh);
        mesh.specularity = m_cache->MeshSpecularity.Get();
        mesh.ambience = m_cache->MeshAmbience.Get();
        mesh.bump = m_cache->MeshBump.Get();
    }
}

void Application::UpdatePost()
{
    const int selectedTexture = m_cache->TextureSelected.Get();
    if (selectedTexture != m_selectedPost)
    {
        m_selectedPost = selectedTexture;
        GetEngine()->SetPostTexture(static_cast<Texture::Post>(selectedTexture));
    }

    GetEngine()->SetDepthNear(m_cache->DepthNear.Get());
    GetEngine()->SetDepthFar(m_cache->DepthFar.Get());
}

void Application::UpdateLight()
{
    const int selectedLight = m_cache->LightSelected.Get();
    if(selectedLight != m_selectedLight)
    {
        m_selectedLight = selectedLight;

        auto& light = m_scene->GetLight(m_selectedLight);
        m_cache->LightPosition.SetUpdated(light.position);
        m_cache->LightAttenuation.SetUpdated(light.attenuation);
        m_cache->LightDiffuse.SetUpdated(light.diffuse);
        m_cache->LightSpecular.SetUpdated(light.specular);
        m_cache->LightSpecularity.SetUpdated(light.specularity);
    }
    else if(m_selectedLight >= 0 && m_selectedLight < m_scene->GetLightCount())
    {
        auto& light = m_scene->GetLight(m_selectedLight);
        light.position = m_cache->LightPosition.Get();
        light.attenuation = m_cache->LightAttenuation.Get();
        light.diffuse = m_cache->LightDiffuse.Get();
        light.specular = m_cache->LightSpecular.Get();
        light.specularity = m_cache->LightSpecularity.Get();
    }
}

bool Application::Initialise(HWND hwnd, HINSTANCE hinstance)
{
    m_timer.reset(new Timer());

    m_scene.reset(new Scene());
    if(!m_scene->Initialise())
    {
        Logger::LogError("Scene: Failed to initialise");
        return false;
    }

    std::vector<std::string> engineNames;
    m_engines.resize(MAX_ENGINES);
    m_engines[OPENGL].reset(new OpenglEngine(hwnd, hinstance));
    m_engines[DIRECTX].reset(new DirectxEngine(hwnd));

    // Ensure that all engines can be initialised
    bool failed = false;
    for(unsigned int i = 0; i < m_engines.size(); ++i)
    {
        engineNames.push_back(m_engines[i]->GetName());
        if(i != m_selectedEngine)
        {
            failed |= !InitialiseEngine(m_engines[i].get());
            m_engines[i]->Release();
        }
    }

    failed |= !InitialiseEngine(GetEngine());

    if(failed)
    {
        Logger::LogError("Render Engine: Failed to initialise");
        return false;
    }

    InitialiseCache(engineNames);
    return true;
}

void Application::InitialiseCache(const std::vector<std::string>& engineNames)
{
    m_cache->TextureSelected.Set(m_selectedPost);
    m_cache->EngineSelected.Set(m_selectedEngine);

    m_cache->Engines.Set(engineNames);
    m_cache->Lights.Set(m_scene->GetLightNames());
    m_cache->Meshes.Set(m_scene->GetMeshNames());
    m_cache->Shaders.Set(m_scene->GetShaderNames());
    m_cache->Textures.Set(m_scene->GetPostTextureNames());

    m_cache->DepthNear.SetUpdated(DEPTH_NEAR);
    m_cache->DepthFar.SetUpdated(DEPTH_FAR);
}

RenderEngine* Application::GetEngine() const
{
    return m_engines[m_selectedEngine].get();
}

bool Application::InitialiseEngine(RenderEngine* engine)
{
    if(!engine->Initialize())
    {
        Logger::LogError(engine->GetName() + ": Failed to initialise");
        return false;
    }

    if(!engine->InitialiseScene(m_scene->GetMeshes(), 
        m_scene->GetAlpha(), m_scene->GetShaders(), m_scene->GetTextures()))
    {
        Logger::LogError(engine->GetName() + ": Scene failed to initialise");
        return false;
    }

    return true;
}

void Application::FadeRenderEngine()
{
    if(m_fadeState != NO_FADE &&
       GetEngine()->FadeView(m_fadeState == FADE_IN, FADE_AMOUNT))
    {
        if(m_fadeState == FADE_OUT)
        {
            SwitchRenderEngine(m_cache->EngineSelected.Get());
            m_fadeState = FADE_IN;
        }
        else
        {
            m_fadeState = NO_FADE;
        }
    }
}

void Application::SwitchRenderEngine(int index)
{
    m_engines[m_selectedEngine]->Release();

    m_selectedEngine = index;
    m_selectedShader = NO_INDEX; // allows selected shader to be re-cached
    m_selectedPost = NO_INDEX;   // allows post values to be sent

    if (!GetEngine()->Initialize() || !GetEngine()->ReInitialiseScene())
    {
        Logger::LogError(GetEngine()->GetName() + ": Failed to reinitialise");
    }

    GetEngine()->UpdateView(m_camera->GetWorld());
    GetEngine()->SetFade(0.0f);
    UpdatePost();
}
