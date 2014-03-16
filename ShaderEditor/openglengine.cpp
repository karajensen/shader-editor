////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "openglshader.h"

////////////////////////////////////////////////////
GLfloat vertices[] = {-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f};
GLfloat colours[] = {1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f};
GLfloat vertices2[] = {	0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,1.0f,0.0f,0.0f };
unsigned int vertexArrayObjID[2];
unsigned int vertexBufferObjID[3];
////////////////////////////////////////////////////

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

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
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

    for(GlShader& shader : shaders)
    {
        shader.Release();
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

std::string OpenglEngine::CompileShader(int index)
{
    return m_data->shaders[index].CompileShader(
        m_data->scratchVS, m_data->scratchFS);
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
        m_data->shaders.push_back(GlShader(shader.glslVertexFile, shader.glslFragmentFile));
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

    glUseProgram(shader.GetProgram());

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