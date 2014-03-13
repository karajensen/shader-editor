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

////////////////////////////////////////////////////
GLfloat vertices[] = {-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f};
GLfloat colours[] = {1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f};
GLfloat vertices2[] = {	0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,1.0f,0.0f,0.0f };
unsigned int vertexArrayObjID[2];
unsigned int vertexBufferObjID[3];
////////////////////////////////////////////////////

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

    glm::mat4 projectionMat;         ///< Matrix for 3D rendering
    std::vector<GlShader> shaders;   ///< OpenGL shader objects
    HGLRC hrc;                       ///< Rendering context  
    HDC hdc;                         ///< Device context  
    GLint scratchVS;                 ///< Scratch GLSL Vertex Shader
    GLint scratchFS;                 ///< Scratch GLSL Fragment Shader
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
    vs(NO_INDEX),
    fs(NO_INDEX)
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
        if(vs != NO_INDEX)
        {
            glDetachShader(program, vs);
        }
        if(fs != NO_INDEX)
        {
            glDetachShader(program, fs);
        }
        glDeleteProgram(program);
        program = NO_INDEX;
    }

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

std::string OpenglEngine::CompileShader(int index, const char* source, int size)
{
    std::string errorBuffer;
    GLint success = GL_FALSE;
    glShaderSource(index, 1, &source, &size);
    glCompileShader(index);
    glGetShaderiv(index, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE)
    {
        int errorLength = 0;
        glGetShaderiv(index, GL_INFO_LOG_LENGTH, &errorLength);
        if(errorLength <= 0)
        {
            errorBuffer = "Unknown Error";
        }
        else
        {
            int actualLength = 0;
            errorBuffer.resize(errorLength);
            glGetShaderInfoLog(index, errorLength, &actualLength, &errorBuffer[0]);
            errorBuffer.resize(actualLength);
        }
    }
    return errorBuffer;
}

std::string OpenglEngine::LoadShaderFile(const std::string& path, int& size, std::string& text)
{
    std::string errorBuffer;
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
    if(file.is_open())
    {
        size = static_cast<int>(file.tellg());
        file.seekg(0, std::ios::beg);
        text.resize(size);
        file.read(&text[0], text.size());
        assert(!text.empty());

        file.close();
    }
    else
    {
        errorBuffer = "Could not open file " + path;
    }
    return errorBuffer;
}

bool OpenglEngine::HasCallFailed()
{
    switch(glGetError())
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_VALUE:
        Logger::LogError("OpenGL: GL_INVALID_VALUE");
        return true;
    case GL_INVALID_OPERATION:
        Logger::LogError("OpenGL: GL_INVALID_OPERATION");
        return true;
    default:
        Logger::LogError("OpenGL: Unknown Error");
        return true;
    }
}

std::string OpenglEngine::CompileShader(int index)
{
    std::string errorBuffer;
    GlShader& shader = m_data->shaders[index];

    // Load the vertex and fragment shader files
    int vSize = 0;
    std::string vText;
    errorBuffer = LoadShaderFile(shader.vsFilepath, vSize, vText);
    if(!errorBuffer.empty())
    {
        return "Vertex Shader: " + errorBuffer;
    }

    int fSize = 0;
    std::string fText;
    errorBuffer = LoadShaderFile(shader.fsFilepath, fSize, fText);
    if(!errorBuffer.empty())
    {
        return "Fragment Shader: " + errorBuffer;
    }

    // Test compilation on the scratch shaders before overwriting current
    errorBuffer = CompileShader(m_data->scratchVS, vText.c_str(), vSize);
    if(!errorBuffer.empty())
    {
        return "Vertex Shader: " + errorBuffer;
    }

    errorBuffer = CompileShader(m_data->scratchFS, fText.c_str(), fSize);
    if(!errorBuffer.empty())
    {
        return "Fragment Shader: " + errorBuffer;
    }

    // Create the actual shader/program
    shader.Release();
    shader.vs = glCreateShader(GL_VERTEX_SHADER);
    shader.fs = glCreateShader(GL_FRAGMENT_SHADER);
    shader.program = glCreateProgram();
    CompileShader(shader.vs, vText.c_str(), vSize);
    CompileShader(shader.fs, fText.c_str(), fSize);

    // TO DO: Allow each shader to have a map of its own 
    // values which are read in from the vertex shader 'in' variables
    glBindAttribLocation(shader.program, 0, "in_Position");
    if(HasCallFailed())
    {
        return "Failed to bind attribute in_Position";
    }
    glBindAttribLocation(shader.program, 1, "in_Color");
    if(HasCallFailed())
    {
        return "Failed to bind attribute in_Color";
    }

    // Attach both shaders to the program
	glAttachShader(shader.program, shader.vs);
    if(HasCallFailed())
    {
        return "Failed to attach vertex shader";
    }

    glAttachShader(shader.program, shader.fs);
    if(HasCallFailed())
    {
        return "Failed to attach fragment shader";
    }

    // Link the program and test everything is good to go
    GLint linkSuccess = GL_FALSE;
    glLinkProgram(shader.program);
    glGetProgramiv(shader.program, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE)
    {
        const int bufferSize = 1024;
        errorBuffer.resize(bufferSize);
        glGetProgramInfoLog(shader.program, bufferSize, 0, &errorBuffer[0]);
        return errorBuffer;
    }

    return std::string();
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

    //////////////////////////////////////////////////////////////////////////////
    glGenVertexArrays(2, &vertexArrayObjID[0]);
    glBindVertexArray(vertexArrayObjID[0]);
    glGenBuffers(2, vertexBufferObjID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
 
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
 
    glBindVertexArray(vertexArrayObjID[1]);
    glGenBuffers(1, &vertexBufferObjID[2]);
 
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[2]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
 
    glBindVertexArray(0);
    //////////////////////////////////////////////////////////////////////////////

    return true;
}

void OpenglEngine::BeginRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void OpenglEngine::Render(const std::vector<Light>& lights)
{
    GlShader& shader = m_data->shaders[0];

    glUseProgram(shader.program);

    //////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(vertexArrayObjID[0]);	// First VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw first object 
    glBindVertexArray(vertexArrayObjID[1]);	// select second VAO
    glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw second object
    glBindVertexArray(0);
    //////////////////////////////////////////////////////////////////////////////
}

void OpenglEngine::EndRender()
{
    SwapBuffers(m_data->hdc); 
}

std::string OpenglEngine::GetName() const
{
    return "OpenGL";
}