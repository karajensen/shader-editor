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

#define SELECTED_ENGINE DIRECTX
//#define SELECTED_ENGINE OPENGL
#define SELECTED_MAP PostProcessing::FINAL_MAP


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
    m_selectedMap(SELECTED_MAP),
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
    GetEngine()->Render(m_scene->GetLights(), m_scene->GetPost());

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
    GetEngine()->WriteToShader(name, text);

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

    const int selectedMap = m_cache->PostMapSelected.Get();
    if (selectedMap != m_selectedMap)
    {
        m_selectedMap = selectedMap;
        m_scene->SetPostMap(selectedMap);
    }

    if (m_cache->SaveLights.Get())
    {
        m_scene->SaveLightsToFile();
        m_cache->SaveLights.Set(false);
    }

    if (m_cache->SaveMeshes.Get())
    {
        m_scene->SaveMeshesToFile();
        m_cache->SaveMeshes.Set(false);
    }

    if (m_cache->SavePost.Get())
    {
        m_scene->SavePostProcessingtoFile();
        m_cache->SavePost.Set(false);
    }

    m_cache->FramesPerSec.Set(m_timer->GetFPS());
    m_cache->DeltaTime.Set(m_timer->GetDeltaTime());
    m_cache->MousePosition.Set(m_mousePosition);
    m_cache->MouseDirection.Set(m_mouseDirection);
}

void Application::UpdateMesh()
{
    const int selectedMesh = m_cache->MeshSelected.Get();
    if(selectedMesh != m_selectedMesh)
    {
        m_selectedMesh = selectedMesh;
        auto& mesh = m_scene->GetMesh(m_selectedMesh);

        m_cache->MeshSpecularity.SetUpdated(mesh.specularity);
        m_cache->MeshAmbience.SetUpdated(mesh.ambience);
        m_cache->MeshGlow.SetUpdated(mesh.glow);
        m_cache->MeshBump.SetUpdated(mesh.bump);
        m_cache->MeshShader.SetUpdated(m_scene->GetShader(mesh.shaderIndex).name);
    }
    else if(m_selectedMesh >= 0 && m_selectedMesh < m_scene->GetMeshCount())
    {
        auto& mesh = m_scene->GetMesh(m_selectedMesh);
        mesh.specularity = m_cache->MeshSpecularity.Get();
        mesh.ambience = m_cache->MeshAmbience.Get();
        mesh.bump = m_cache->MeshBump.Get();
        mesh.glow = m_cache->MeshGlow.Get();
    }
}

void Application::UpdatePost()
{
    PostProcessing& post = m_scene->GetPost();
    post.dofDistance = m_cache->DOFDistance.Get();
    post.dofFade = m_cache->DOFFade.Get();
    post.blurAmount = m_cache->BlurAmount.Get();
    post.blurStep = m_cache->BlurStep.Get();
    post.glowAmount = m_cache->GlowAmount.Get();
    post.contrast = m_cache->Contrast.Get();
    post.saturation = m_cache->Saturation.Get();
    post.fogColour = m_cache->FogColour.Get();
    post.fogDistance = m_cache->FogDistance.Get();
    post.fogFade = m_cache->FogFade.Get();
    post.depthFar = m_cache->DepthFar.Get();
    post.depthNear = m_cache->DepthNear.Get();
    post.minimumColour = m_cache->MinimumColour.Get();
    post.maximumColour = m_cache->MaximumColour.Get();
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
    m_scene->SetPostMap(m_selectedMap);
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
    m_cache->PostMapSelected.Set(m_selectedMap);
    m_cache->EngineSelected.Set(m_selectedEngine);

    m_cache->Engines.Set(engineNames);
    m_cache->Lights.Set(m_scene->GetLightNames());
    m_cache->Meshes.Set(m_scene->GetMeshNames());
    m_cache->Shaders.Set(m_scene->GetShaderNames());
    m_cache->PostMaps.Set(m_scene->GetPostMapNames());

    const PostProcessing& post = m_scene->GetPost();
    m_cache->DepthNear.SetUpdated(post.depthNear);
    m_cache->DepthFar.SetUpdated(post.depthFar);
    m_cache->DOFDistance.SetUpdated(post.dofDistance);
    m_cache->DOFFade.SetUpdated(post.dofFade);
    m_cache->BlurAmount.SetUpdated(post.blurAmount);
    m_cache->BlurStep.SetUpdated(post.blurStep);
    m_cache->GlowAmount.SetUpdated(post.glowAmount);
    m_cache->Contrast.SetUpdated(post.contrast);
    m_cache->Saturation.SetUpdated(post.saturation);
    m_cache->FogFade.SetUpdated(post.fogFade);
    m_cache->FogDistance.SetUpdated(post.fogDistance);
    m_cache->FogColour.SetUpdated(post.fogColour);
    m_cache->MinimumColour.SetUpdated(post.minimumColour);
    m_cache->MaximumColour.SetUpdated(post.maximumColour);
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
        m_scene->GetShaders(), m_scene->GetTextures()))
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
    m_selectedMap = NO_INDEX;    // allows post values to be re-cached

    if (!GetEngine()->Initialize() || !GetEngine()->ReInitialiseScene())
    {
        Logger::LogError(GetEngine()->GetName() + ": Failed to reinitialise");
    }

    GetEngine()->UpdateView(m_camera->GetWorld());
    GetEngine()->SetFade(0.0f);
}
