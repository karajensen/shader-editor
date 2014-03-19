////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "timer.h"
#include "openglengine.h"
#include "directxengine.h"
#include "scene.h"
#include <AntTweakBar.h>
#include <windowsx.h>

namespace
{
    const bool OPENGL_START = true;
    const std::string TWEAK_BAR_NAME("ShaderEditor");

    /**
    * Anttweakbar button callback for toggling through render engines
    * @param clientData The user given data
    */
    void TW_CALL ButtonToggleRenderEngine(void *clientData)
    {
        (static_cast<Application*>(clientData))->ToggleRenderEngine();
    }
}

Application::Application() :
    m_engine(nullptr),
    m_runApplication(true),
    m_tweakbar(nullptr),
    m_showTweakBar(false)
{
}

Application::~Application()
{
    RemoveTweakBar();
}

bool Application::Run()
{
    WPARAM keyDown;
    MSG msg;
    m_timer->StartTimer();

    while(m_runApplication)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if((GetAsyncKeyState(VK_ESCAPE) & 0x8000) ||
               (msg.message == WM_QUIT))
            {
                return true;
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
    return true;
}

void Application::HandleKeyPress(const WPARAM& keydown)
{
    if(keydown == VK_F1)
    {
        ToggleTweakBar();
    }
    else if(keydown == VK_F2)
    {
        ToggleRenderEngine();
    }
    else
    {
        TwKeyPressed(toascii(keydown),0);
    }
}

void Application::HandleInputEvents(WPARAM& keydown, const MSG& msg)
{
    switch(msg.message)
    {
    case WM_KEYDOWN:
        keydown = msg.wParam;
        break;
    case WM_KEYUP:
        HandleKeyPress(keydown);
        break;
    case WM_LBUTTONUP:
        TwMouseButton(TW_MOUSE_RELEASED,TW_MOUSE_LEFT);
        break;
    case WM_RBUTTONUP:
        TwMouseButton(TW_MOUSE_RELEASED,TW_MOUSE_RIGHT);
        break;
    case WM_LBUTTONDOWN:
        TwMouseButton(TW_MOUSE_PRESSED,TW_MOUSE_LEFT);
        break;
    case WM_RBUTTONDOWN:
        TwMouseButton(TW_MOUSE_PRESSED,TW_MOUSE_RIGHT);
        break;
    case WM_MOUSEMOVE:
        TwMouseMotion(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
        break;
    }
}

void Application::TickApplication()
{
    m_scene->Update();
    m_engine->BeginRender();
    m_engine->Render(m_scene->GetLights());
    TwDraw();
    m_engine->EndRender();
}

bool Application::Initialise(HWND hwnd)
{
    m_timer.reset(new Timer());
    m_scene.reset(new Scene());
    m_directx.reset(new DirectxEngine(hwnd));
    m_opengl.reset(new OpenglEngine(hwnd));

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
        m_scene->GetAlpha(), m_scene->GetShaders()))
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
        m_scene->GetAlpha(), m_scene->GetShaders()))
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
    InitialiseTweakBar(OPENGL_START);
    return true;
}

void Application::ToggleRenderEngine()
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

    InitialiseTweakBar(useOpenGL);
}

void Application::InitialiseTweakBar(bool opengl)
{
    RemoveTweakBar();

    if(opengl)
    {
        TwInit(TW_OPENGL_CORE, nullptr);
    }
    else
    {
        TwInit(TW_DIRECT3D11, m_directx->GetDevice());
    }

    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_tweakbar = TwNewBar(TWEAK_BAR_NAME.c_str());
    TwDefine(" TW_HELP visible=false ");

    const int border = 10;
    std::ostringstream stream;
    const std::string visible(m_showTweakBar ? "true" : "false");
    const std::string barLabel("Shader Editor: " + m_engine->GetName());

    stream << TWEAK_BAR_NAME << " label='" << barLabel << "' " 
        << "position='" << border << " " << border << "' "
        << "size='200 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=false "
        << "fontsize=2 fontresizable=false visible=" << visible << " ";
    TwDefine(stream.str().c_str());

    TwAddButton(m_tweakbar, "Toggle Render Engine",
        &ButtonToggleRenderEngine, this, "");

    m_scene->InitialiseTweakBar(m_tweakbar);
}

void Application::RemoveTweakBar()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Application::ToggleTweakBar()
{
    m_showTweakBar = !m_showTweakBar;
    const std::string visible(m_showTweakBar ? "true" : "false");
    TwDefine((" " + TWEAK_BAR_NAME + " visible=" + visible + " ").c_str());
}