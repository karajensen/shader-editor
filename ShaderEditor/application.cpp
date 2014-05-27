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
#include <windowsx.h>

namespace
{
    const bool OPENGL_START = true;

    const float CAMERA_MOVE_SPEED = 40.0f; ///< Speed the camera will translate
    const float CAMERA_ROT_SPEED = 2.0f;   ///< Speed the camera will rotate
    const float CAMERA_SIDE_SPEED = 20.0f; ///< Speed the camera will strafe
}

Application::Application() :
    m_engine(nullptr),
    m_camera(new Camera()),
    m_mousePressed(false),
    m_switchEngine(false)
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
        if(m_switchEngine)
        {
            m_switchEngine = false;
            SwitchRenderEngine();
        }

        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if(IsKeyDown(VK_ESCAPE) || msg.message == WM_QUIT)
            {
                runApplication = false;
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
    case WM_MOUSEMOVE:
        HandleMouseMovement(msg);
        break;
    }
}

void Application::HandleKeyPress(const WPARAM& keydown)
{
    switch(keydown)
    {
    case VK_F2:
        ToggleRenderEngine();
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
        m_engine->UpdateView(m_camera->GetWorld());
    }

    m_scene->Update();
    m_engine->BeginRender();
    m_engine->Render(m_scene->GetLights());
    m_engine->EndRender();

    m_mouseDirection.x = 0;
    m_mouseDirection.y = 0;
}

bool Application::Initialise(HWND hwnd, HINSTANCE hinstance)
{
    m_timer.reset(new Timer());
    m_scene.reset(new Scene());
    m_directx.reset(new DirectxEngine(hwnd));
    m_opengl.reset(new OpenglEngine(hwnd, hinstance));

    if(!m_scene->Initialise())
    {
        Logger::LogError("Scene: Failed to initialise");
        return false;
    }

    if(!m_opengl->Initialize())
    {
        Logger::LogError("OpenGL: Failed to initialise");
        return false;
    }

    if(!m_opengl->InitialiseScene(m_scene->GetMeshes(), 
        m_scene->GetAlpha(), m_scene->GetShaders(), m_scene->GetTextures()))
    {
        Logger::LogError("OpenGL: Scene failed to initialise");
        return false;
    }
    
    if(!m_directx->Initialize())
    {
        Logger::LogError("DirectX: Failed to initialise");
        return false;
    }

    if(!m_directx->InitialiseScene(m_scene->GetMeshes(), 
        m_scene->GetAlpha(), m_scene->GetShaders(), m_scene->GetTextures()))
    {
        Logger::LogError("DirectX: Scene failed to initialise");
        return false;
    }

    if(OPENGL_START)
    {
        m_engine = m_opengl.get();
    }
    else
    {
        m_engine = m_directx.get();
    }

    return true;
}

void Application::ToggleRenderEngine()
{
    m_switchEngine = true;
}

void Application::SwitchRenderEngine()
{
    const bool useOpenGL = !(m_engine == m_opengl.get());
    if(useOpenGL)
    {
        m_engine = m_opengl.get();
    }
    else
    {
        m_engine = m_directx.get();
    }

    // Reinitialise the engine as it has lost focus
    if(!m_engine->Initialize() || !m_engine->ReInitialiseScene())
    {
        Logger::LogError(m_engine->GetName() + ": Failed to reinitialise");
    }

    m_engine->UpdateView(m_camera->GetWorld());
}
