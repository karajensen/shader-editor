////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "openglshader.h"
#include "openglmesh.h"

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

    glm::mat4 projection;            ///< Projection matrix
    glm::mat4 view;                  ///< Camera View matrix
    std::vector<GlMesh> meshes;      ///< OpenGL mesh objects
    std::vector<GlShader> shaders;   ///< OpenGL shader objects
    HGLRC hrc;                       ///< Rendering context  
    HDC hdc;                         ///< Device context  
    GLint scratchVS;                 ///< Scratch GLSL Vertex Shader
    GLint scratchFS;                 ///< Scratch GLSL Fragment Shader
    std::vector<GLuint> vao;         ///< IDs for the Vertex Array Objects
};

OpenglData::OpenglData() :
    hdc(nullptr),
    hrc(nullptr),
    scratchVS(NO_INDEX),
    scratchFS(NO_INDEX)
{
}

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
    for(GlMesh& mesh : meshes)
    {
        mesh.Release();
    }

    for(GlShader& shader : shaders)
    {
        shader.Release();
    }

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

    if(!vao.empty())
    {
        glDeleteBuffers(vao.size(), &vao[0]);
        vao.clear();
    }

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

    const int colourBits = 32;
    const int depthBits = 24;

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = colourBits;
    pfd.cDepthBits = depthBits;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Initialise Glew with a temporary opengl context
    int tempPixelFormat = ChoosePixelFormat(m_data->hdc, &pfd);
    if(tempPixelFormat == 0)
    {
        Logger::LogError("OpenGL: GLEW Pixel Format unsupported");
        return false;
    }
  
    if(!SetPixelFormat(m_data->hdc, tempPixelFormat, &pfd))
    {
        Logger::LogError("OpenGL: GLEW Set Pixel Format failed");
        return false;
    }

    HGLRC tempOpenGLContext = wglCreateContext(m_data->hdc); 
    wglMakeCurrent(m_data->hdc, tempOpenGLContext);
    if(!tempOpenGLContext || HasCallFailed())
    {
        Logger::LogError("OpenGL: GLEW Temporary context creation failed");
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK || HasCallFailed())
    {
        Logger::LogError("OpenGL: GLEW Initialization failed");
        return false;
    }

    if(!wglewIsSupported("WGL_ARB_create_context") || HasCallFailed())
    { 
        Logger::LogError("OpenGL: GLEW No support for 3.0+");
        return false;
    }  

    // Create the actual opengl context
    const int pixelAttributes[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB,     TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_SUPPORT_OPENGL_ARB,     TRUE,
        WGL_DOUBLE_BUFFER_ARB,      TRUE,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         colourBits,
        WGL_DEPTH_BITS_ARB,         depthBits,
        WGL_STENCIL_BITS_ARB,       0,
        0,                          0
    };

    UINT numFormats = 0;
    int pixelFormat = 0;
    wglChoosePixelFormatARB(m_data->hdc, pixelAttributes, 0, 1, &pixelFormat, &numFormats);
    if(pixelFormat == 0 || numFormats == 0 || HasCallFailed())
    {
        Logger::LogInfo("OpenGL: Choose pixel format failed");
        return false;
    }
    
    if(!SetPixelFormat(m_data->hdc, pixelFormat, &pfd))
    {
        Logger::LogInfo("OpenGL: Set pixel format failed");
        return false;
    }
             
    int contextAttributes[] = 
    {  
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB,
        0, 0  
    };  

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempOpenGLContext);

    m_data->hrc = wglCreateContextAttribsARB(m_data->hdc, 0, contextAttributes);
    wglMakeCurrent(m_data->hdc, m_data->hrc);
    if(!m_data->hrc || HasCallFailed())
    {
        Logger::LogInfo("OpenGL: Context creation failed");
        return false;
    }

    // Scratch required to check for compilation errors before overwriting
    if(m_data->scratchVS == NO_INDEX)
    {
        m_data->scratchVS = glCreateShader(GL_VERTEX_SHADER);
    }
    if(m_data->scratchFS == NO_INDEX)
    {
        m_data->scratchFS = glCreateShader(GL_FRAGMENT_SHADER);
    }
    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Scratch shader generation failed");
        return false;
    }

    // Initialise the opengl environment
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(CAMERA_NEAR, CAMERA_FAR);

    m_data->projection = glm::perspective(FIELD_OF_VIEW, 
        WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT),
        CAMERA_NEAR, CAMERA_FAR);

    // Determine version accepted and if there was anything wrong
    int minor, major;
    glGetIntegerv(GL_MAJOR_VERSION, &major); 
    glGetIntegerv(GL_MINOR_VERSION, &minor); 
    if(minor == -1 || major == -1)
    {
        Logger::LogInfo("OpenGL: Version not supported");
        return false;
    }

    if(!HasCallFailed())
    {
        std::stringstream stream;
        stream << "OpenGL: Verson " << major << "." << minor << " successful";
        Logger::LogInfo(stream.str());
        return true;
    }

    return false;
}

std::string OpenglEngine::CompileShader(int index)
{
    return m_data->shaders[index].CompileShader(
        m_data->scratchVS, m_data->scratchFS);
}

bool OpenglEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                   const std::vector<Mesh>& alpha, 
                                   const std::vector<Shader>& shaders)
{
    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        m_data->shaders.push_back(GlShader(shader.index, 
            shader.glslVertexFile, shader.glslFragmentFile));
    }

    m_data->meshes.reserve(meshes.size());
    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(GlMesh(mesh));
        break;
    }

    return ReInitialiseScene();
}

bool OpenglEngine::ReInitialiseScene()
{
    for(unsigned int i = 0; i < m_data->shaders.size(); ++i)
    {
        const std::string result = CompileShader(i);
        if(!result.empty())
        {
            Logger::LogError("OpenGL: " + result);
            return false;
        }
    }

    m_data->vao.resize(m_data->meshes.size());
    glGenVertexArrays(m_data->vao.size(), &m_data->vao[0]);
    for(unsigned int i = 0; i < m_data->meshes.size(); ++i)
    {
        m_data->meshes[i].Initialise(m_data->vao[i]);
    }

    return true;
}

void OpenglEngine::BeginRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglEngine::Render(const std::vector<Light>& lights)
{
    GlShader& shader = m_data->shaders[0];
    shader.SetAsActive();

    const glm::mat4 viewMat = m_data->view;
    const glm::mat4 projMat = m_data->projection;
    shader.SendUniformMatrix("viewMat", viewMat);
    shader.SendUniformMatrix("projMat", projMat);
    
    const float testing = 0.5f;
    shader.SendUniformFloat("testing", testing);

    for(GlMesh& mesh : m_data->meshes)
    {
        mesh.PreRender();
        shader.EnableAttributes();
        mesh.Render();
    }
}

void OpenglEngine::EndRender()
{
    SwapBuffers(m_data->hdc); 
}

std::string OpenglEngine::GetName() const
{
    return "OpenGL";
}

void OpenglEngine::UpdateView(const Matrix& world)
{
    // RHS Matrix
    // | 11 12 13 x |
    // | 21 22 23 y |
    // | 31 32 33 z |
    // | 0  0  0  1 |

    glm::mat4 viewMatrix;

    viewMatrix[0][0] = world.m11;  
    viewMatrix[0][1] = world.m12;
    viewMatrix[0][2] = world.m13;
    viewMatrix[0][3] = world.m14;

    viewMatrix[1][0] = world.m21;
    viewMatrix[1][1] = world.m22;
    viewMatrix[1][2] = world.m23;
    viewMatrix[1][3] = world.m24;

    viewMatrix[2][0] = world.m31;
    viewMatrix[2][1] = world.m32;
    viewMatrix[2][2] = world.m33;
    viewMatrix[2][3] = world.m34;

    m_data->view = glm::inverse(viewMatrix);
}