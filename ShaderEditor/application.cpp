////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "openglengine.h"
#include "common.h"

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
    while(m_runApplication)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //Handle any windows messages
            if(msg.message == WM_QUIT)
            {
                return true;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
        else
        {
            //Tick the application
            m_engine->Render();
        }
    }
    return true;
}

bool Application::Initialise(HWND hwnd)
{
    m_opengl.reset(new OpenglEngine());
    if(!m_opengl->Initialize(hwnd))
    {
        Logger::LogError("OpenGL Failed to initialise");
        return false;
    }









    m_engine = m_opengl.get();

    return true;
}
