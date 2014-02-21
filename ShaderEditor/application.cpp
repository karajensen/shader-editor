////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "timer.h"
#include "openglengine.h"
#include "scene.h"
#include <AntTweakBar.h>
#include <windowsx.h>

namespace
{
    const std::string TWEAK_BAR_NAME("ShaderEditor");
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

void Application::HandleInputEvents(WPARAM& keydown, const MSG& msg)
{
    switch(msg.message)
    {
    case WM_KEYDOWN:
        keydown = msg.wParam;
        break;
    case WM_KEYUP:
        if(keydown == VK_F1)
        {
            ToggleTweakBar();
        }
        TwKeyPressed(toascii(keydown),0);
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

    TwDraw();

    m_engine->EndRender();
}

bool Application::Initialise(HWND hwnd)
{
    m_opengl.reset(new OpenglEngine());
    if(!m_opengl->Initialize(hwnd))
    {
        Logger::LogError("OpenGL Failed to initialise");
        return false;
    }

    const bool initialiseWithOpenGL = true;
    SetRenderEngine(initialiseWithOpenGL);

    m_timer.reset(new Timer());
    m_scene.reset(new Scene(m_tweakbar));

    return m_scene->Initialise();
}

void Application::SetRenderEngine(bool opengl)
{
    m_engine = m_opengl.get();
    InitializeTweakBar(opengl);
}

void Application::InitializeTweakBar(bool opengl)
{
    RemoveTweakBar();
    std::string barLabel("Shader Editor: ");

    if(opengl)
    {
        barLabel += "OpenGL";
        TwInit(TW_OPENGL_CORE, nullptr);
    }
    else
    {
        barLabel += "DirectX";
        TwInit(TW_DIRECT3D11, nullptr);
    }
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    m_tweakbar = TwNewBar(TWEAK_BAR_NAME.c_str());
    TwDefine(" TW_HELP visible=false ");

    const int border = 10;
    std::ostringstream stream;
    stream << TWEAK_BAR_NAME << " label='" << barLabel << "' " 
        << "position='" << border << " " << border << "' "
        << "size='200 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=false "
        << "fontsize=2 fontresizable=false visible=false ";
    TwDefine(stream.str().c_str());
    m_showTweakBar = false;
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