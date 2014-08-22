////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "openglshader.h"
#include "openglmesh.h"
#include "opengltexture.h"
#include "opengltarget.h"
#include <array>

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

    HGLRC hrc = nullptr;             ///< Rendering context  
    HDC hdc = nullptr;               ///< Device context                            
    GlRenderTarget backBuffer;       ///< Render target for the back buffer
    GlRenderTarget sceneTarget;      ///< Render target for the main scene
    GlRenderTarget normalTarget;     ///< Render target for the scene normal/depth map
    GlShader postShader;             ///< Shader for post processing the scene
    GlShader normalShader;           ///< Shader for rendering normals/depth for the scene
    GlMesh quad;                     ///< Quad to render the final post processed scene onto
    glm::vec2 cameraDepth;           ///< Camera near and far values
    glm::vec3 cameraPosition;        ///< Position of the camera
    glm::mat4 projection;            ///< Projection matrix
    glm::mat4 view;                  ///< View matrix
    glm::mat4 viewProjection;        ///< View projection matrix
    bool isBackfaceCull = true;      ///< Whether backface culling is currently active
    int selectedShader = NO_INDEX;   ///< Currently active shader for rendering
    float fadeAmount = 0.0f;         ///< the amount to fade the scene by

    std::array<float, Texture::MAX_POST> postAlpha;   ///< Visibility of post textures
    std::vector<std::unique_ptr<GlTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<GlMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<GlShader>> shaders;   ///< Shaders shared by all meshes
};

OpenglData::OpenglData() :
    quad("ScreenQuad"),
    sceneTarget("SceneTarget"),
    normalTarget("NormalTarget"),
    backBuffer("BackBuffer", true),
    cameraDepth(DEPTH_NEAR, DEPTH_FAR),
    postShader(POST_NAME, POST_VERT_PATH, POST_FRAG_PATH),
    normalShader(NORMAL_NAME, NORM_VERT_PATH, NORM_FRAG_PATH)
{
}

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
    selectedShader = NO_INDEX;
    isBackfaceCull = true;
    fadeAmount = 0.0f;

    for(auto& texture : textures)
    {
        texture->Release();
    }

    for(auto& mesh : meshes)
    {
        mesh->Release();
    }

    for(auto& shader : shaders)
    {
        shader->Release();
    }

    backBuffer.Release();
    sceneTarget.Release();
    normalTarget.Release();
    postShader.Release();
    normalShader.Release();
    quad.Release();

    wglMakeCurrent(nullptr, nullptr);
    if(hrc)
    {
        wglDeleteContext(hrc);
        hrc = nullptr;
    }
}

OpenglEngine::OpenglEngine(HWND hwnd, HINSTANCE hinstance) :
    m_data(new OpenglData()),
    m_hwnd(hwnd)
{
    // Create temporary window used only for Glew
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX)); 
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = nullptr;
    wc.hInstance = hinstance; 
    wc.lpszClassName = "GlewWindow";
    RegisterClassEx(&wc); 

    m_temporaryHwnd = CreateWindowEx(WS_EX_APPWINDOW,
        "GlewWindow", TEXT("GlewWindow"), 0, 0, 0, 0, 0, 
        nullptr, nullptr, hinstance, nullptr);
}

OpenglEngine::~OpenglEngine()
{
    Release();
}

void OpenglEngine::Release()
{
    m_data->Release();
}

bool OpenglEngine::Initialize()
{
    // SetPixelFormat can only be called once per window and is required for Glew.
    // Initialise Glew with a temporary opengl context and temporary window.
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd369049%28v=vs.85%29.aspx

    HDC tempHdc = GetDC(m_temporaryHwnd);
    m_data->hdc = GetDC(m_hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int tempPixelFormat = ChoosePixelFormat(tempHdc, &pfd);
    if(tempPixelFormat == 0)
    {
        Logger::LogError("OpenGL: GLEW Pixel Format unsupported");
        return false;
    }
  
    if(!SetPixelFormat(tempHdc, tempPixelFormat, &pfd))
    {
        Logger::LogError("OpenGL: GLEW Set Pixel Format failed");
        return false;
    }

    HGLRC tempOpenGLContext = wglCreateContext(tempHdc); 
    wglMakeCurrent(tempHdc, tempOpenGLContext);
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

    // Determine the supported attributes for opengl. Do this before deleting 
    // the temporary context as HasCallFailed() does not work without one
    const int pixelAttributes[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_SAMPLE_BUFFERS_ARB,     GL_TRUE,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_SUPPORT_OPENGL_ARB,     TRUE,
        WGL_STENCIL_BITS_ARB,       0,
        WGL_SAMPLES_ARB,            MULTISAMPLING_COUNT,
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

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempOpenGLContext);

    // Create the actual opengl context
    int contextAttributes[] = 
    {  
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB,
        0, 0  
    };  

    m_data->hrc = wglCreateContextAttribsARB(m_data->hdc, 0, contextAttributes);
    wglMakeCurrent(m_data->hdc, m_data->hrc);
    if(!m_data->hrc || HasCallFailed())
    {
        Logger::LogError("OpenGL: Context creation failed");
        return false;
    }

    int minor, major;
    glGetIntegerv(GL_MAJOR_VERSION, &major); 
    glGetIntegerv(GL_MINOR_VERSION, &minor); 
    if(minor == -1 || major == -1)
    {
        Logger::LogError("OpenGL: Version not supported");
        return false;
    }

    // Create the render targets
    if(!m_data->backBuffer.Initialise() ||
       !m_data->sceneTarget.Initialise() ||
       !m_data->normalTarget.Initialise())
    {
        Logger::LogError("OpenGL: Could not create render targets");
        return false;
    }

    // Create the normal/depth shader
    std::string errorBuffer = m_data->normalShader.CompileShader();
    if(!errorBuffer.empty())
    {
        Logger::LogError("OpenGL: Normal shader failed: " + errorBuffer);
        return false;
    }

    // Create the post processing quad and shader
    if(!m_data->quad.Initialise())
    {
        Logger::LogError("OpenGL: Scene quad failed to initialise");
        return false;
    }

    errorBuffer = m_data->postShader.CompileShader();
    if(!errorBuffer.empty())
    {
        Logger::LogError("OpenGL: Post shader failed: " + errorBuffer);
        return false;
    }

    if(!m_data->postShader.HasTextureSlot(Texture::SCENE_TEXTURE) ||
       !m_data->postShader.HasTextureSlot(Texture::NORMAL_TEXTURE))
    {
        Logger::LogError("OpenGL: Post shader does not have required texture slots");
        return false;
    }

    // Initialise the opengl environment
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 
    SetPostTexture(Texture::SCENE_TEXTURE);

    m_data->projection = glm::perspective(FIELD_OF_VIEW, 
        WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT),
        FRUSTRUM_NEAR, FRUSTRUM_FAR);

    if(!HasCallFailed())
    {
        std::stringstream stream;
        stream << "OpenGL: Version " << glGetString(GL_VERSION) << " successful";
        Logger::LogInfo(stream.str());
        return true;
    }

    return false;
}

std::string OpenglEngine::CompileShader(int index)
{
    return m_data->shaders[index]->CompileShader();
}

bool OpenglEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                   const std::vector<Mesh>& alpha, 
                                   const std::vector<Shader>& shaders,
                                   const std::vector<Texture>& textures)
{
    m_data->textures.reserve(textures.size());
    for(const Texture& texture : textures)
    {
        m_data->textures.push_back(std::unique_ptr<GlTexture>(
            new GlTexture(texture.path)));
    }

    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        m_data->shaders.push_back(std::unique_ptr<GlShader>(
            new GlShader(shader.name, shader.glslVertexFile, 
                shader.glslFragmentFile, shader.index)));
    }

    m_data->meshes.reserve(meshes.size());
    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(std::unique_ptr<GlMesh>(
            new GlMesh(&mesh)));
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
            Logger::LogError("OpenGL: " + 
                m_data->shaders[i]->GetName() + ": " + result);
            return false;
        }
    }

    for(auto& mesh : m_data->meshes)
    {
        if(!mesh->Initialise())
        {
            Logger::LogError("OpenGL: Failed to re-initialise mesh");
            return false;
        }
    }

    for(auto& texture : m_data->textures)
    {
        if(!texture->Initialise())
        {
            Logger::LogError("OpenGL: Failed to re-initialise texture");
            return false;
        }
    }

    Logger::LogInfo("OpenGL: Re-Initialised");
    return true;
}

bool OpenglEngine::FadeView(bool in, float amount)
{
    m_data->fadeAmount += in ? amount : -amount;

    if(in && m_data->fadeAmount >= 1.0f)
    {
        m_data->fadeAmount = 1.0f;
        return true;
    }
    else if(!in && m_data->fadeAmount <= 0.0f)
    {
        m_data->fadeAmount = 0.0f;
        return true;
    }
    return false;
}

void OpenglEngine::Render(const std::vector<Light>& lights)
{
    m_data->selectedShader = NO_INDEX; // always reset due to post shader

    // Render the scene
    m_data->sceneTarget.SetActive();
    for(auto& mesh : m_data->meshes)
    {
        UpdateShader(mesh->GetMesh(), lights);
        SetTextures(mesh->GetTextureIDs());
        SetBackfaceCull(mesh->ShouldBackfaceCull());
    
        mesh->PreRender();
        m_data->shaders[mesh->GetShaderID()]->EnableAttributes();
        mesh->Render();
    }

    // Render the normal/depth map
    m_data->normalTarget.SetActive();
    m_data->normalShader.SetActive();
    m_data->normalShader.SendUniformMatrix("viewProjection", m_data->viewProjection);
    m_data->normalShader.SendUniformFloat("cameraDepth", &m_data->cameraDepth.x, 2);
    for(auto& mesh : m_data->meshes)
    {
        SetBackfaceCull(mesh->ShouldBackfaceCull());
    
        mesh->PreRender();
        m_data->normalShader.EnableAttributes();
        mesh->Render();
    }

    // Render the scene as a texture to the backbuffer
    SetBackfaceCull(false);
    m_data->backBuffer.SetActive();
    m_data->postShader.SetActive();
    RenderPostProcessing();

    SwapBuffers(m_data->hdc); 
}

void OpenglEngine::RenderPostProcessing()
{
    m_data->postShader.SendUniformFloat("fadeAmount", 
        &m_data->fadeAmount, 1);

    m_data->postShader.SendUniformFloat("sceneAlpha",
        &m_data->postAlpha[Texture::SCENE_TEXTURE], 1);

    m_data->postShader.SendUniformFloat("normalAlpha",
        &m_data->postAlpha[Texture::NORMAL_TEXTURE], 1);

    m_data->postShader.SendUniformFloat("depthAlpha",
        &m_data->postAlpha[Texture::DEPTH_TEXTURE], 1);

    m_data->sceneTarget.SendTexture(Texture::SCENE_TEXTURE);
    m_data->normalTarget.SendTexture(Texture::NORMAL_TEXTURE);

    m_data->quad.PreRender();
    m_data->postShader.EnableAttributes();
    m_data->quad.Render();

    m_data->sceneTarget.ClearTexture(Texture::SCENE_TEXTURE);
    m_data->normalTarget.ClearTexture(Texture::NORMAL_TEXTURE);
}

void OpenglEngine::SetTextures(const std::vector<int>& textureIDs)
{
    auto& shader = m_data->shaders[m_data->selectedShader];

    int slot = 0;
    for(int id : textureIDs)
    {
        if(id != NO_INDEX)
        {
            if(shader->HasTextureSlot(slot))
            {
                shader->SendTexture(slot, m_data->textures[id]->GetID());
                ++slot;
            }
            else
            {
                Logger::LogError("Shader and mesh texture count does not match");
            }
        }
    }
}

void OpenglEngine::UpdateShader(const Mesh& mesh, 
                                const std::vector<Light>& lights)
{
    const int index = mesh.shaderIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        m_data->selectedShader = index;
        shader->SetActive();

        shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        shader->SendUniformFloat("cameraPosition", &m_data->cameraPosition.x, 3);

        shader->SendUniformFloat("lightSpecularity", &lights[0].specularity, 1);
        shader->SendUniformFloat("lightAttenuation", &lights[0].attenuation.x, 3);
        shader->SendUniformFloat("lightPosition", &lights[0].position.x, 3);
        shader->SendUniformFloat("lightDiffuse", &lights[0].diffuse.r, 3);
        shader->SendUniformFloat("lightSpecular", &lights[0].specular.r, 3);
    }

    shader->SendUniformFloat("meshAmbience", &mesh.ambience, 1);
    shader->SendUniformFloat("meshBump", &mesh.bump, 1);
    shader->SendUniformFloat("meshSpecularity", &mesh.specularity, 1);
}

std::string OpenglEngine::GetName() const
{
    return "OpenGL";
}

void OpenglEngine::UpdateView(const Matrix& world)
{
    glm::mat4 view;

    view[0][0] = world.m11;  
    view[1][0] = world.m12;
    view[2][0] = world.m13;
    view[3][0] = world.m14;

    view[0][1] = world.m21;
    view[1][1] = world.m22;
    view[2][1] = world.m23;
    view[3][1] = world.m24;

    view[0][2] = world.m31;
    view[1][2] = world.m32;
    view[2][2] = world.m33;
    view[3][2] = world.m34;

    m_data->cameraPosition.x = world.m14;
    m_data->cameraPosition.y = world.m24;
    m_data->cameraPosition.z = world.m34;

    m_data->view = glm::inverse(view);

    // Model pivot points exist at the origin: world matrix is the identity
    m_data->viewProjection = m_data->projection * m_data->view;
}

void OpenglEngine::SetBackfaceCull(bool shouldCull)
{
    if(shouldCull != m_data->isBackfaceCull)
    {
        m_data->isBackfaceCull = shouldCull;
        shouldCull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
}

std::string OpenglEngine::GetShaderText(int index) const
{
    if(index == m_data->shaders.size())
    {
        return m_data->postShader.GetText();
    }
    else if(index == m_data->shaders.size() + 1)
    {
        return m_data->normalShader.GetText();
    }
    return m_data->shaders[index]->GetText();
}

std::string OpenglEngine::GetShaderAssembly(int index)
{
    if(index == m_data->shaders.size())
    {
        return m_data->postShader.GetAssembly();
    }
    else if(index == m_data->shaders.size() + 1)
    {
        return m_data->normalShader.GetAssembly();
    }
    return m_data->shaders[index]->GetAssembly();
}

void OpenglEngine::SetFade(float value)
{
    m_data->fadeAmount = value;
}

void OpenglEngine::SetPostTexture(Texture::Post post)
{
    m_data->postAlpha.assign(0.0f);
    m_data->postAlpha[post] = 1.0f;
}

void OpenglEngine::SetDepthNear(float value)
{
    m_data->cameraDepth[0] = value;
}

void OpenglEngine::SetDepthFar(float value)
{
    m_data->cameraDepth[1] = value;
}