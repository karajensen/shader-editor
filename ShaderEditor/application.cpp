////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "timer.h"
#include "openglengine.h"
#include "scene.h"
#include <AntTweakBar.h>

Application::Application() :
    m_engine(nullptr),
    m_runApplication(true),
    m_tweakbar(nullptr),
    m_showTweakbar(true)
{
}

Application::~Application()
{
    RemoveTweakBar();
}

bool Application::Run()
{
    MSG msg;
    m_timer->StartTimer();

    while(m_runApplication)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT ||
              (GetAsyncKeyState(VK_ESCAPE) & 0x8000))
            {
                return true;
            }
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

void Application::TickApplication()
{
    m_engine->BeginRender();

    if(m_showTweakbar)
    {
        TwDraw();
    }

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

    if(opengl)
    {
        TwInit(TW_OPENGL_CORE, nullptr);
    }
    else
    {
        //note requires device pointer
        TwInit(TW_DIRECT3D11, nullptr);
    }

    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string barname = "ShaderEditor";
    m_tweakbar = TwNewBar(barname.c_str());

    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Shader Editor' " 
        << "position='" << border << " " << border << "' "
        << "size='200 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=false "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());
}

void Application::RemoveTweakBar()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}