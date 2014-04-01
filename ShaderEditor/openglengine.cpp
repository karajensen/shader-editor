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
    std::vector<unsigned int> vao;   ///< IDs for the Vertex Array Objects
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

    const int colourDepth = 32;
    const int depthBufferBits = 32;
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, colourDepth, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        depthBufferBits, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

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
    glewExperimental = GL_TRUE;
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
    
    // Initialise the opengl environment
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    m_data->projection = glm::perspective(FIELD_OF_VIEW, 
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

    if(!HasCallFailed())
    {
        std::stringstream stream;
        stream << "OpenGL: Verson " << version[0] << "." << version[1] << " successful";
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
        m_data->shaders.push_back(GlShader(shader.glslVertexFile, shader.glslFragmentFile));
    }

    m_data->meshes.reserve(meshes.size());
    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(GlMesh());
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

    const glm::mat4 viewProj = m_data->projection * m_data->view;
    shader.SendUniformMatrix("viewProjection", viewProj);

    const float testing = 0.5f;
    shader.SendUniformFloat("testing", testing);

    for(GlMesh& mesh : m_data->meshes)
    {
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