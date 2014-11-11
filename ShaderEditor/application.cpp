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
    GetEngine()->Render(*m_scene);

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
    m_cache->Timer.Set(m_timer->GetTotalTime());
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

        m_cache->Mesh[MESH_AMBIENCE].SetUpdated(mesh.ambience);
        m_cache->Mesh[MESH_SPECULARITY].SetUpdated(mesh.specularity);
        m_cache->Mesh[MESH_BUMP].SetUpdated(mesh.bump);
        m_cache->Mesh[MESH_GLOW].SetUpdated(mesh.glow);
        m_cache->MeshShader.SetUpdated(m_scene->GetShader(mesh.shaderIndex).name);
    }
    else if(m_selectedMesh >= 0 && m_selectedMesh < m_scene->GetMeshCount())
    {
        auto& mesh = m_scene->GetMesh(m_selectedMesh);
        mesh.specularity = m_cache->Mesh[MESH_SPECULARITY].Get();
        mesh.ambience = m_cache->Mesh[MESH_AMBIENCE].Get();
        mesh.bump = m_cache->Mesh[MESH_BUMP].Get();
        mesh.glow = m_cache->Mesh[MESH_GLOW].Get();
    }

    const int selectedWater = m_cache->WaterSelected.Get();
    if(selectedWater != m_selectedWater)
    {
        m_selectedWater = selectedWater;
        auto& water = m_scene->GetWater(m_selectedWater);

        m_cache->Water[WATER_BUMP].SetUpdated(water.bump);
        m_cache->Water[WATER_BUMP_SPEED].SetUpdated(water.bumpSpeed);
        m_cache->Water[WATER_SPEED].SetUpdated(water.speed);
        m_cache->Water[WATER_DEEP_R].SetUpdated(water.deepColour.r);
        m_cache->Water[WATER_DEEP_G].SetUpdated(water.deepColour.g);
        m_cache->Water[WATER_DEEP_B].SetUpdated(water.deepColour.b);
        m_cache->Water[WATER_SHALLOW_R].SetUpdated(water.shallowColour.r);
        m_cache->Water[WATER_SHALLOW_G].SetUpdated(water.shallowColour.g);
        m_cache->Water[WATER_SHALLOW_B].SetUpdated(water.shallowColour.b);
        m_cache->Water[WATER_REFLECTION_R].SetUpdated(water.reflectionTint.r);
        m_cache->Water[WATER_REFLECTION_G].SetUpdated(water.reflectionTint.g);
        m_cache->Water[WATER_REFLECTION_B].SetUpdated(water.reflectionTint.b);
        m_cache->Water[WATER_REFLECTION].SetUpdated(water.reflection);
        m_cache->Water[WATER_FRESNAL_FACTOR].SetUpdated(water.fresnalFactor);
        m_cache->Water[WATER_OFFSET_U].SetUpdated(water.textureOffset.x);
        m_cache->Water[WATER_OFFSET_V].SetUpdated(water.textureOffset.y);
        m_cache->WaveAmount.SetUpdated(static_cast<int>(water.waves.size()));
    }
    else if(m_selectedWater >= 0 && m_selectedWater < m_scene->GetWaterCount())
    {
        auto& water = m_scene->GetWater(m_selectedWater);
        water.bump = m_cache->Water[WATER_BUMP].Get();
        water.bumpSpeed = m_cache->Water[WATER_BUMP_SPEED].Get();
        water.speed = m_cache->Water[WATER_SPEED].Get();
        water.deepColour.r = m_cache->Water[WATER_DEEP_R].Get();
        water.deepColour.g = m_cache->Water[WATER_DEEP_G].Get();
        water.deepColour.b = m_cache->Water[WATER_DEEP_B].Get();
        water.shallowColour.r = m_cache->Water[WATER_SHALLOW_R].Get();
        water.shallowColour.g = m_cache->Water[WATER_SHALLOW_G].Get();
        water.shallowColour.b = m_cache->Water[WATER_SHALLOW_B].Get();
        water.reflectionTint.r = m_cache->Water[WATER_REFLECTION_R].Get();
        water.reflectionTint.g = m_cache->Water[WATER_REFLECTION_G].Get();
        water.reflectionTint.b = m_cache->Water[WATER_REFLECTION_B].Get();
        water.reflection = m_cache->Water[WATER_REFLECTION].Get();
        water.fresnalFactor = m_cache->Water[WATER_FRESNAL_FACTOR].Get();
        water.textureOffset.x = m_cache->Water[WATER_OFFSET_U].Get();
        water.textureOffset.y = m_cache->Water[WATER_OFFSET_V].Get();
    }

    if (m_selectedWater != NO_INDEX)
    {
        auto& water = m_scene->GetWater(m_selectedWater);
        const int selectedWave = m_cache->WaveSelected.Get();

        if(selectedWave != m_selectedWave)
        {
            m_selectedWave = selectedWave;
            auto& wave = water.waves[m_selectedWave];

            m_cache->Wave[WAVE_AMPLITUDE].SetUpdated(wave.amplitude);
            m_cache->Wave[WAVE_FREQUENCY].SetUpdated(wave.frequency);
            m_cache->Wave[WAVE_SPEED].SetUpdated(wave.speed);
            m_cache->Wave[WAVE_DIRECTION_X].SetUpdated(wave.directionX);
            m_cache->Wave[WAVE_DIRECTION_Z].SetUpdated(wave.directionZ);
        }
        else if(m_selectedWave >= 0 && m_selectedWave < static_cast<int>(water.waves.size()))
        {
            auto& wave = water.waves[m_selectedWave];
            wave.amplitude = m_cache->Wave[WAVE_AMPLITUDE].Get();
            wave.frequency = m_cache->Wave[WAVE_FREQUENCY].Get();
            wave.speed = m_cache->Wave[WAVE_SPEED].Get();
            wave.directionX = m_cache->Wave[WAVE_DIRECTION_X].Get();
            wave.directionZ = m_cache->Wave[WAVE_DIRECTION_Z].Get();
        }
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
    post.depthFar = m_cache->DepthFar.Get();
    post.depthNear = m_cache->DepthNear.Get();
    post.fogColour.r = m_cache->Fog[FOG_RED].Get();
    post.fogColour.g = m_cache->Fog[FOG_GREEN].Get();
    post.fogColour.b = m_cache->Fog[FOG_BLUE].Get();
    post.fogDistance = m_cache->Fog[FOG_DISTANCE].Get();
    post.fogFade = m_cache->Fog[FOG_FADE].Get();
    post.minimumColour.r = m_cache->MinColour[RED].Get();
    post.minimumColour.g = m_cache->MinColour[GREEN].Get();
    post.minimumColour.b = m_cache->MinColour[BLUE].Get();
    post.maximumColour.r = m_cache->MaxColour[RED].Get();
    post.maximumColour.g = m_cache->MaxColour[GREEN].Get();
    post.maximumColour.b = m_cache->MaxColour[BLUE].Get();
}

void Application::UpdateLight()
{
    const int selectedLight = m_cache->LightSelected.Get();
    if(selectedLight != m_selectedLight)
    {
        m_selectedLight = selectedLight;

        auto& light = m_scene->GetLight(m_selectedLight);
        m_cache->Light[LIGHT_POSITION_X].SetUpdated(light.position.x);
        m_cache->Light[LIGHT_POSITION_Y].SetUpdated(light.position.y);
        m_cache->Light[LIGHT_POSITION_Z].SetUpdated(light.position.z);
        m_cache->Light[LIGHT_ATTENUATION_X].SetUpdated(light.attenuation.x);
        m_cache->Light[LIGHT_ATTENUATION_Y].SetUpdated(light.attenuation.y);
        m_cache->Light[LIGHT_ATTENUATION_Z].SetUpdated(light.attenuation.z);
        m_cache->Light[LIGHT_DIFFUSE_R].SetUpdated(light.diffuse.r);
        m_cache->Light[LIGHT_DIFFUSE_G].SetUpdated(light.diffuse.g);
        m_cache->Light[LIGHT_DIFFUSE_B].SetUpdated(light.diffuse.b);
        m_cache->Light[LIGHT_SPECULAR_R].SetUpdated(light.specular.r);
        m_cache->Light[LIGHT_SPECULAR_G].SetUpdated(light.specular.g);
        m_cache->Light[LIGHT_SPECULAR_B].SetUpdated(light.specular.b);
        m_cache->Light[LIGHT_SPECULARITY].SetUpdated(light.specularity);
    }
    else if(m_selectedLight >= 0 && m_selectedLight < m_scene->GetLightCount())
    {
        auto& light = m_scene->GetLight(m_selectedLight);
        light.position.x = m_cache->Light[LIGHT_POSITION_X].Get();
        light.position.y = m_cache->Light[LIGHT_POSITION_Y].Get();
        light.position.z = m_cache->Light[LIGHT_POSITION_Z].Get();
        light.attenuation.x = m_cache->Light[LIGHT_ATTENUATION_X].Get();
        light.attenuation.y = m_cache->Light[LIGHT_ATTENUATION_Y].Get();
        light.attenuation.z = m_cache->Light[LIGHT_ATTENUATION_Z].Get();
        light.diffuse.r = m_cache->Light[LIGHT_DIFFUSE_R].Get();
        light.diffuse.g = m_cache->Light[LIGHT_DIFFUSE_G].Get();
        light.diffuse.b = m_cache->Light[LIGHT_DIFFUSE_B].Get();
        light.specular.r = m_cache->Light[LIGHT_SPECULAR_R].Get();
        light.specular.g = m_cache->Light[LIGHT_SPECULAR_G].Get();
        light.specular.b = m_cache->Light[LIGHT_SPECULAR_B].Get();
        light.specularity = m_cache->Light[LIGHT_SPECULARITY].Get();
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
    m_cache->Waters.Set(m_scene->GetWaterNames());

    m_cache->WaveAmount.SetUpdated(m_scene->GetWaterCount() > 0 ? 
        static_cast<int>(m_scene->GetWater(0).waves.size()) : 0);

    const PostProcessing& post = m_scene->Post();
    m_cache->DepthNear.SetUpdated(post.depthNear);
    m_cache->DepthFar.SetUpdated(post.depthFar);
    m_cache->DOFDistance.SetUpdated(post.dofDistance);
    m_cache->DOFFade.SetUpdated(post.dofFade);
    m_cache->BlurAmount.SetUpdated(post.blurAmount);
    m_cache->BlurStep.SetUpdated(post.blurStep);
    m_cache->GlowAmount.SetUpdated(post.glowAmount);
    m_cache->Contrast.SetUpdated(post.contrast);
    m_cache->Saturation.SetUpdated(post.saturation);
    m_cache->Fog[FOG_RED].SetUpdated(post.fogColour.r);
    m_cache->Fog[FOG_GREEN].SetUpdated(post.fogColour.g);
    m_cache->Fog[FOG_BLUE].SetUpdated(post.fogColour.b);
    m_cache->Fog[FOG_DISTANCE].SetUpdated(post.fogDistance);
    m_cache->Fog[FOG_FADE].SetUpdated(post.fogFade);
    m_cache->MinColour[RED].SetUpdated(post.minimumColour.r);
    m_cache->MinColour[GREEN].SetUpdated(post.minimumColour.g);
    m_cache->MinColour[BLUE].SetUpdated(post.minimumColour.b);
    m_cache->MaxColour[RED].SetUpdated(post.maximumColour.r);
    m_cache->MaxColour[GREEN].SetUpdated(post.maximumColour.g);
    m_cache->MaxColour[BLUE].SetUpdated(post.maximumColour.b);
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

    if(!engine->InitialiseScene(*m_scene))
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
