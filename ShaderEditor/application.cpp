////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "common.h"
#include "timer.h"
#include "openglengine.h"
#include "lightmanager.h"
#include "diagnostics.h"

Application::Application() :
    m_engine(nullptr),
    m_runApplication(true)
{
}

Application::~Application()
{
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

    m_diagnostics->Render();

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
    m_lighting.reset(new LightManager());
    m_diagnostics.reset(new Diagnostics());

    m_lighting->Initialise();

    return true;
}

void Application::SetRenderEngine(bool opengl)
{
    m_engine = m_opengl.get();
    m_diagnostics->InitializeTweakBar(opengl);
}