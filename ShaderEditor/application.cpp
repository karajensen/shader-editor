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
#include "sceneModifier.h"
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
    m_selectedEngine(SELECTED_ENGINE),
    m_camera(new Camera()),
    m_timer(new Timer()),
    m_scene(new Scene()),
    m_modifier(new SceneModifier(*m_scene, *m_timer, cache, SELECTED_MAP))
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
                m_modifier->SetApplicationRunning(false);
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

        m_modifier->SetSelectedEngine(index);
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
    m_modifier->Tick(*GetEngine(), m_mousePosition, m_mouseDirection);

    m_mouseDirection.x = 0;
    m_mouseDirection.y = 0;
}

bool Application::Initialise(HWND hwnd, HINSTANCE hinstance)
{    
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

    m_modifier->Initialise(engineNames, m_selectedEngine);
    return true;
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
            SwitchRenderEngine(m_modifier->GetSelectedEngine());
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
