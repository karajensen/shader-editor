////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "opengl/include/glew.h"
#include "opengl/include/wglew.h"
#include "opengl/include/GL.h"
#include "opengl/include/GLU.h"
#include "common.h"

OpenglEngine::OpenglEngine() :
    m_hwnd(nullptr),
    m_hdc(nullptr),
    m_hrc(nullptr)
{
}

OpenglEngine::~OpenglEngine()
{
    if(m_hdc)
    {
        wglMakeCurrent(m_hdc, 0);
    }
    if(m_hrc)
    {
        wglDeleteContext(m_hrc);
    }
    if(m_hdc && m_hwnd)
    {
        ReleaseDC(m_hwnd, m_hdc);
    }
}

bool OpenglEngine::Initialize(HWND hwnd)
{
    m_hwnd = hwnd;
    m_hdc = GetDC(m_hwnd);

    PIXELFORMATDESCRIPTOR pfd;  
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA; 
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if(pixelFormat == 0)
    {
        Logger::LogError("OpenGL: PIXELFORMATDESCRIPTOR unsupported");
        return false;
    }
  
    if(!SetPixelFormat(m_hdc, pixelFormat, &pfd))
    {
        Logger::LogError("OpenGL: SetPixelFormat failed");
        return false;
    }

    // Create a temporary OpenGL 2.1 context
    HGLRC tempOpenGLContext = wglCreateContext(m_hdc); 
    wglMakeCurrent(m_hdc, tempOpenGLContext);

    // Initialize GLEW
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        Logger::LogError("OpenGL: GLEW Initialization failed");
        return false;
    }

    int attributes[] = 
    {  
        //OpenGL Version: Major.Minor
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        0  
    };  

    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    { 
        m_hrc = wglCreateContextAttribsARB(m_hdc, nullptr, attributes);
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(tempOpenGLContext); 
        wglMakeCurrent(m_hdc, m_hrc); 
    }  
    else
    {  
        // No support for OpenGL 3.0+
        m_hrc = tempOpenGLContext; 
        Logger::LogInfo("OpenGL: No support for 3.0+");
    }  

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    int glVersion[2] = {-1, -1}; 
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
    if(glVersion[0] == -1 || glVersion[1] == -1)
    {
        Logger::LogInfo("OpenGL: Incorrect versioning");
        return false;
    }

    const float version = glVersion[0] + (glVersion[1] * 0.1f);
    Logger::LogInfo("OpenGL: Verson " + StringCast(version) + " initialized");
    return true;
}

void OpenglEngine::Render()
{
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
    SwapBuffers(m_hdc); 
}