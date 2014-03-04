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
#include <assert.h>

/**
* Holds information for an individual opengl shader
*/
struct GlShader
{
    /**
    * Constructor
    */
    GlShader();

    /**
    * Destructor
    */
    ~GlShader();

    /**
    * Releases the shader object
    */
    void Release();

    std::string vsFilepath;  ///< Path to the vertex shader file
    std::string fsFilepath;  ///< Path to the fragment shader file
	GLint program;           ///< Shader program
	GLint vs;                ///< GLSL Vertex Shader
	GLint fs;                ///< GLSL Fragment Shader
};

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

    glm::mat4 projectionMat;           ///< Matrix for 3D rendering
    std::vector<GlShader> shaders;     ///< OpenGL shader objects
    HGLRC hrc;                         ///< Rendering context  
    HDC hdc;                           ///< Device context  
	GLint scratchVS;                   ///< Scratch GLSL Vertex Shader
	GLint scratchFS;                   ///< Scratch GLSL Fragment Shader
};

OpenglData::OpenglData() :
    hdc(nullptr),
    hrc(nullptr),
    scratchVS(NO_INDEX),
    scratchFS(NO_INDEX)
{
}

GlShader::GlShader() :
    program(NO_INDEX),
    vs(glCreateShader(GL_VERTEX_SHADER)),
    fs(glCreateShader(GL_FRAGMENT_SHADER))
{
}

OpenglData::~OpenglData()
{
    Release();

    if(scratchVS != NO_INDEX)
    {
        glDeleteShader(scratchVS);
        scratchVS = NO_INDEX;
    }

    if(scratchFS != NO_INDEX)
    {
        glDeleteShader(scratchFS);
        scratchFS = NO_INDEX;
    }
}

GlShader::~GlShader()
{
    Release();

    if(vs != NO_INDEX)
    {
        glDeleteShader(vs);
        vs = NO_INDEX;
    }

    if(fs != NO_INDEX)
    {
        glDeleteShader(fs);
        fs = NO_INDEX;
    }
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

void GlShader::Release()
{
    if(program != NO_INDEX)
    {
        glDeleteProgram(program);
        program = NO_INDEX;
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

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 

    m_data->projectionMat = glm::perspective(FOV, 
        WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT),
        CAMERA_NEAR, CAMERA_FAR);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

    if(m_data->scratchVS == NO_INDEX)
    {
        m_data->scratchVS = glCreateShader(GL_VERTEX_SHADER);
    }
    if(m_data->scratchFS == NO_INDEX)
    {
        m_data->scratchFS = glCreateShader(GL_FRAGMENT_SHADER);
    }

    return true;
}

std::string OpenglEngine::CompileShader(int index)
{
    GlShader& shader = m_data->shaders[index];

    auto loaderShaderFile = [&](const std::string& path, GLint& size, std::string& text)
    {
        std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
        if(file.is_open())
        {
            size = static_cast<GLint>(file.tellg());
            file.seekg(0, std::ios::beg);
            text.resize(size);
            file.read(&text[0], text.size());
            assert(!text.empty());

            file.close();
        }
        else
        {
            Logger::LogError("OpenGL: Could not open file " + path);
        }
    };

    GLint vSize, fSize;
    std::string vText, fText;
    loaderShaderFile(shader.vsFilepath, vSize, vText);
    loaderShaderFile(shader.fsFilepath, fSize, fText);

    std::string errorBuffer;
    auto compileShader = [&](GLint glIndex, const char* source, GLint size) -> bool
    {
        GLint status = NO_INDEX;
        glShaderSource(glIndex, 1, &source, &size);
        glCompileShader(glIndex);
        glGetShaderiv(glIndex, GL_COMPILE_STATUS, &status);
        if(!status)
        {
            int errorLength = 0;
            glGetShaderiv(glIndex, GL_INFO_LOG_LENGTH, &errorLength);
            if(errorLength <= 0)
            {
                errorBuffer = "Unknown Error";
            }
            else
            {
                int actualLength = 0;
                errorBuffer.resize(errorLength);
                glGetShaderInfoLog(glIndex, errorLength, &actualLength, &errorBuffer[0]);
                errorBuffer.resize(actualLength);
            }
            return false;
        }
        return true;
    };

    // Test compilation before overwriting
    if(!compileShader(m_data->scratchVS, vText.c_str(), vSize))
    {
        return "Vertex Shader: " + errorBuffer;
    }
    if(!compileShader(m_data->scratchFS, fText.c_str(), fSize))
    {
        return "Fragment Shader: " + errorBuffer;
    }

    shader.Release();
    shader.program = glCreateProgram();
    compileShader(shader.vs, vText.c_str(), vSize);
    compileShader(shader.vs, fText.c_str(), fSize);
	glAttachShader(shader.program, shader.vs);
    glAttachShader(shader.program, shader.fs);

    return "";
}

bool OpenglEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                   const std::vector<Mesh>& alpha, 
                                   const std::vector<Shader>& shaders)
{
    // Generate GLSL OpenGL Shaders
    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        const int index = m_data->shaders.size();
        m_data->shaders.push_back(GlShader());
        m_data->shaders[index].vsFilepath = shader.glslVertexFile;
        m_data->shaders[index].fsFilepath = shader.glslFragmentFile;
        const std::string result = CompileShader(index);
        if(!result.empty())
        {
            Logger::LogError("OpenGL: " + result);
            return false;
        }
    }


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

std::string OpenglEngine::GetName() const
{
    return "OpenGL";
}