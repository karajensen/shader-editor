////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "opengl/include/glew.h"
#include "opengl/include/wglew.h"
#include "opengl/include/GL.h"
#include "opengl/include/GLU.h"
#include "opengl/glm/glm.hpp"
#include "opengl/glm/gtc/matrix_transform.hpp"
#include "common.h"
#include <iomanip>

/**
* Internal data for the opengl rendering engine
*/
struct OpenglData
{
    /**
    * Constructor
    */
    OpenglData();

    /**
    * Destructor
    */
    ~OpenglData();

    /**
    * Releases the device/context
    */
    void Release();

    glm::mat4 projectionMat;  ///< Matrix for 3D rendering

    HGLRC hrc;  ///< Rendering context  
    HDC hdc;    ///< Device context  
};

OpenglData::OpenglData() :
    hdc(nullptr),
    hrc(nullptr)
{
}

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
    wglMakeCurrent(nullptr, nullptr);
    if(hrc)
    {
        wglDeleteContext(hrc);
        hrc = nullptr;
    }
}

OpenglEngine::OpenglEngine(HWND hwnd) :
    m_data(new OpenglData()),
    m_hwnd(hwnd)
{
}

OpenglEngine::~OpenglEngine()
{
}

bool OpenglEngine::Initialize()
{
    m_data->Release();
    m_data->hdc = GetDC(m_hwnd);

    PIXELFORMATDESCRIPTOR pfd;  
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA; 
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_data->hdc, &pfd);
    if(pixelFormat == 0)
    {
        Logger::LogError("OpenGL: PIXELFORMATDESCRIPTOR unsupported");
        return false;
    }
  
    if(!SetPixelFormat(m_data->hdc, pixelFormat, &pfd))
    {
        Logger::LogError("OpenGL: SetPixelFormat failed");
        return false;
    }

    // Create a temporary OpenGL context
    HGLRC tempOpenGLContext = wglCreateContext(m_data->hdc); 
    wglMakeCurrent(m_data->hdc, tempOpenGLContext);

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
        WGL_CONTEXT_FLAGS_ARB, 0, 0  
    };  

    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    { 
        m_data->hrc = wglCreateContextAttribsARB(m_data->hdc, nullptr, attributes);
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(tempOpenGLContext); 
        wglMakeCurrent(m_data->hdc, m_data->hrc); 
    }  
    else
    {  
        Logger::LogError("OpenGL: No support for 3.0+");
        return false;
    }  

    int version[2] = {-1, -1}; 
    glGetIntegerv(GL_MAJOR_VERSION, &version[0]); 
    glGetIntegerv(GL_MINOR_VERSION, &version[1]); 
    if(version[0] == -1 || version[1] == -1)
    {
        Logger::LogInfo("OpenGL: Version not supported");
        return false;
    }
    
    std::stringstream stream;
    stream << "OpenGL: Verson " << version[0] << "." << version[1] << " successful";
    Logger::LogInfo(stream.str());

    // Initialise the scene
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 

    m_data->projectionMat = glm::perspective(FOV, 
        WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT),
        CAMERA_NEAR, CAMERA_FAR);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

    return true;
}

void OpenglEngine::BeginRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void OpenglEngine::Render(const std::vector<Light>& lights)
{

}

void OpenglEngine::EndRender()
{
    SwapBuffers(m_data->hdc); 
}

std::string OpenglEngine::CompileShader(int index)
{
    return "";
}

bool OpenglEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                   const std::vector<Mesh>& alpha, 
                                   const std::vector<Shader>& shaders)
{

    return true;
}

std::string OpenglEngine::GetName() const
{
    return "OpenGL";
}