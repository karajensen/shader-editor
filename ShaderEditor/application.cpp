////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "application.h"
#include "openglengine.h"

Application::Application() :
    m_engine(nullptr)
{
}

Application::~Application()
{
}

bool Application::Run()
{


    return true;
}

bool Application::Initialise()
{
    m_opengl.reset(new OpenglEngine());


    return true;
}
