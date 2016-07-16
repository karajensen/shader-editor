////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "postprocessing.h"
#include "timer.h"
#include "openglengine.h"
#include "directxengine.h"
#include "vulcanengine.h"
#include "scene.h"
#include "camera.h"
#include "cache.h"
#include "appGUI.h"
#include <windowsx.h>

//#define SELECTED_ENGINE DIRECTX
//#define SELECTED_ENGINE OPENGL
#define SELECTED_ENGINE VULCAN
#define SELECTED_MAP PostProcessing::FINAL_MAP

Application::Application() :
    m_selectedEngine(SELECTED_ENGINE),
    m_camera(std::make_unique<Camera>()),
    m_timer(std::make_unique<Timer>()),
    m_scene(std::make_unique<Scene>())
{
}

Application::~Application() = default;

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
                m_modifier->SetApplicationRunning(false);
            }
            HandleInputEvents(keyDown, msg);
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
        else
        {
            m_timer->UpdateTimer();
            HandleKeyDown();
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
    if (keypress == VK_F1)
    {
        m_camera->ToggleAutoMove();
    }
    if (keypress == VK_F2)
    {
        int index = m_selectedEngine + 1;
        if(index >= static_cast<int>(m_engines.size()))
        {
            index = 0;
        }
        ForceRenderEngine(index);
    }
    else if (keypress == '1')
    {
        m_scene->SetPostMap(PostProcessing::FINAL_MAP);
    }
    else if (keypress == '2')
    {
        m_scene->SetPostMap(PostProcessing::SCENE_MAP);
    }
    else if (keypress == '3')
    {
        m_scene->SetPostMap(PostProcessing::DEPTH_MAP);
    }
    else if (keypress == '4')
    {
        m_scene->SetPostMap(PostProcessing::BLUR_MAP);
    }
    else if (keypress == '5')
    {
        m_scene->SetPostMap(PostProcessing::BLOOM_MAP);
    }
    else if (keypress == '6')
    {
        m_scene->SetPostMap(PostProcessing::FOG_MAP);
    }
    else if (keypress == '7')
    {
        m_scene->SetPostMap(PostProcessing::DOF_MAP);
    }
    else if (keypress == '0')
    {
        GetEngine().ToggleWireframe();
    }
}

void Application::HandleKeyDown()
{
    const float deltaTime = m_timer->GetDeltaTime();

    if(m_mousePressed)
    {
        m_camera->Rotate(m_mouseDirection, deltaTime);
    }
    if (IsKeyDown('W'))
    {
        m_camera->Forward(deltaTime);
    }
    if (IsKeyDown('S'))
    {
        m_camera->Forward(-deltaTime);
    }
    if (IsKeyDown('A'))
    {
        m_camera->Right(-deltaTime);
    }
    if (IsKeyDown('D'))
    {
        m_camera->Right(deltaTime);
    }
    if (IsKeyDown('Q'))
    {
        m_camera->Up(deltaTime);
    }
    if (IsKeyDown('E'))
    {
        m_camera->Up(-deltaTime);
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

    m_mousePosition.x = x;
    m_mousePosition.y = y;
}

void Application::TickApplication()
{
    m_modifier->Tick(GetEngine());

    if(m_camera->Update(m_timer->GetDeltaTime()))
    {
        GetEngine().UpdateView(m_camera->GetWorld());
    }

    m_scene->Tick(m_timer->GetDeltaTime(), *m_camera);

    GetEngine().Render(*m_scene, m_timer->GetTotalTime());

    m_scene->PostTick();

    FadeRenderEngine();

    m_mouseDirection.x = 0;
    m_mouseDirection.y = 0;
}

bool Application::Initialise(HWND hwnd, 
                             HINSTANCE hinstance, 
                             std::shared_ptr<Cache> cache)
{    
    if(!m_scene->Initialise(m_camera->Position()))
    {
        Logger::LogError("Scene: Failed to initialise");
        return false;
    }

    std::vector<std::string> engineNames;
    m_engines.resize(MAX_ENGINES);
    m_engines[OPENGL].reset(new OpenglEngine(hwnd, hinstance));
    m_engines[DIRECTX].reset(new DirectxEngine(hwnd));
    m_engines[VULCAN].reset(new VulcanEngine(hwnd));

    // Ensure that all engines can be initialised
    bool failed = false;
    for(unsigned int i = 0; i < m_engines.size(); ++i)
    {
        engineNames.push_back(m_engines[i]->GetName());
        if(i != m_selectedEngine)
        {
            failed |= !InitialiseEngine(*m_engines[i]);
            m_engines[i]->Release();
        }
    }

    failed |= !InitialiseEngine(GetEngine());

    if(failed)
    {
        Logger::LogError("Render Engine: Failed to initialise");
        return false;
    }

    m_modifier = std::make_unique<AppGUI>(
        *m_scene, *m_timer, *m_camera, cache, SELECTED_MAP,
        [this](){ ForceRenderEngine(m_selectedEngine); });

    m_modifier->Initialise(engineNames, m_selectedEngine);

    return true;
}

RenderEngine& Application::GetEngine() const
{
    return *m_engines[m_selectedEngine];
}

bool Application::InitialiseEngine(RenderEngine& engine)
{
    if(!engine.Initialize())
    {
        Logger::LogError(engine.GetName() + ": Failed to initialise");
        return false;
    }

    if(!engine.InitialiseScene(*m_scene))
    {
        Logger::LogError(engine.GetName() + ": Scene failed to initialise");
        return false;
    }

    return true;
}

void Application::FadeRenderEngine()
{
    const int selectedEngine = m_modifier->GetSelectedEngine();
    const float fadeAmount = 0.02f;

    if (selectedEngine != m_selectedEngine)
    {
        m_fadeState = FADE_OUT;
    }

    if(m_fadeState != NO_FADE &&
       GetEngine().FadeView(m_fadeState == FADE_IN, fadeAmount))
    {
        if(m_fadeState == FADE_OUT)
        {
            SwitchRenderEngine(selectedEngine);
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
    auto& engine = GetEngine();

    if (!engine.Initialize() || !engine.ReInitialiseScene())
    {
        Logger::LogError(engine.GetName() + ": Failed to reinitialise");
    }

    engine.UpdateView(m_camera->GetWorld());
    engine.SetFade(0.0f);
}

void Application::ForceRenderEngine(int index)
{
    m_modifier->SetSelectedEngine(index);
    SwitchRenderEngine(index);
    m_engines[index]->SetFade(1.0f);
}