////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "openglshader.h"
#include "openglmesh.h"
#include "opengltexture.h"
#include "opengltarget.h"
#include "sceneElements.h"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/regex.hpp"
#include <array>
#include <fstream>

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
    GlRenderTarget blurTargetP1;     ///< Render target for blurring the scene (pass1)
    GlRenderTarget blurTargetP2;     ///< Render target for blurring the scene (pass2)
    GlMesh quad;                     ///< Quad to render the final post processed scene onto
    glm::vec3 cameraPosition;        ///< Position of the camera
    glm::mat4 projection;            ///< Projection matrix
    glm::mat4 view;                  ///< View matrix
    glm::mat4 viewProjection;        ///< View projection matrix
    bool isBackfaceCull = true;      ///< Whether backface culling is currently active
    int selectedShader = NO_INDEX;   ///< Currently active shader for rendering
    float fadeAmount = 0.0f;         ///< the amount to fade the scene by

    std::vector<std::unique_ptr<GlTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<GlMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<GlWater>> waters;     ///< Each water in the scene
    std::vector<std::unique_ptr<GlShader>> shaders;   ///< Shaders shared by all meshes
};

OpenglData::OpenglData() :
    quad("ScreenQuad"),
    sceneTarget("SceneTarget"),
    normalTarget("NormalTarget"),
    blurTargetP1("BlurTargetP1"),
    blurTargetP2("BlurTargetP2"),
    backBuffer("BackBuffer", true)
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

    for(auto& water : waters)
    {
        water->Release();
    }

    for(auto& shader : shaders)
    {
        shader->Release();
    }

    backBuffer.Release();
    sceneTarget.Release();
    blurTargetP1.Release();
    blurTargetP2.Release();
    normalTarget.Release();
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
       !m_data->blurTargetP1.Initialise() ||
       !m_data->blurTargetP2.Initialise() ||
       !m_data->normalTarget.Initialise())
    {
        Logger::LogError("OpenGL: Could not create render targets");
        return false;
    }

    // Create the post processing quad
    if(!m_data->quad.Initialise())
    {
        Logger::LogError("OpenGL: Scene quad failed to initialise");
        return false;
    }

    // Set the blending state
    // Use zero for src alpha to prevent from interferring with glow
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ZERO);

    // Initialise the opengl environment
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 
    SetBackfaceCull(true);
    EnableAlphaBlending(false);

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

bool OpenglEngine::InitialiseScene(const SceneElements& scene)
{
    m_data->textures.reserve(scene.Textures().size());
    for(const Texture& texture : scene.Textures())
    {
        m_data->textures.push_back(std::unique_ptr<GlTexture>(
            new GlTexture(texture.path)));
    }

    m_data->shaders.reserve(scene.Shaders().size());
    for(const Shader& shader : scene.Shaders())
    {
        m_data->shaders.push_back(std::unique_ptr<GlShader>(
            new GlShader(shader)));
    }

    m_data->meshes.reserve(scene.Meshes().size());
    for(const Mesh& mesh : scene.Meshes())
    {
        m_data->meshes.push_back(std::unique_ptr<GlMesh>(
            new GlMesh(&mesh)));
    }

    m_data->waters.reserve(scene.Waters().size());
    for(const Water& water : scene.Waters())
    {
        m_data->waters.push_back(std::unique_ptr<GlWater>(
            new GlWater(&water)));
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

    for(auto& water : m_data->waters)
    {
        if(!water->Initialise())
        {
            Logger::LogError("OpenGL: Failed to re-initialise water");
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

void OpenglEngine::Render(const SceneElements& scene, float timer)
{
    auto renderMesh = [this](GlMesh& mesh)
    {
        SetTextures(mesh.GetTextureIDs());
        SetBackfaceCull(mesh.ShouldBackfaceCull());
        mesh.PreRender();
        m_data->shaders[mesh.GetShaderID()]->EnableAttributes();
        mesh.Render();
    };

    // Render the scene/glow map
    m_data->sceneTarget.SetActive();
    for (auto& mesh : m_data->meshes)
    {
        UpdateShader(mesh->GetMesh(), scene.Lights());
        renderMesh(*mesh);
    }

    EnableAlphaBlending(true);
    for (auto& water : m_data->waters)
    {
        UpdateShader(water->GetWater(), scene.Lights(), timer);
        renderMesh(*water);
    }
    EnableAlphaBlending(false);

    // Render the normal/depth map
    m_data->normalTarget.SetActive();
    for (auto& mesh : m_data->meshes)
    {
        UpdateShader(mesh->GetMesh(), scene.Post());
        renderMesh(*mesh);
    }
    for (auto& water : m_data->waters)
    {
        UpdateShader(water->GetWater(), scene.Post());
        renderMesh(*water);
    }

    // Render the post processing
    SetBackfaceCull(false);
    RenderSceneBlur(scene.Post());
    RenderPostProcessing(scene.Post());
    SwapBuffers(m_data->hdc); 
}

void OpenglEngine::RenderSceneBlur(const PostProcessing& post)
{
    auto& blurShader = m_data->shaders[BLUR_SHADER_INDEX];
    blurShader->SetActive();
    blurShader->SendUniformFloat("blurStep", &post.blurStep, 1);
    blurShader->SendUniformFloat("blurAmount", &post.blurAmount, 1);
    blurShader->SendUniformFloat("weightMain", &post.weights[0], 1);
    blurShader->SendUniformFloat("weightOffset", &post.weights[1], 4);

    // Render first horizontal blur pass
    m_data->blurTargetP1.SetActive();
    float horizontalPass = 1.0f;
    float verticalPass = 0.0f;
    blurShader->SendUniformFloat("verticalPass", &verticalPass, 1);
    blurShader->SendUniformFloat("horizontalPass", &horizontalPass, 1);

    blurShader->SendTexture(0, m_data->sceneTarget.GetTextureID(), false, true);
    m_data->quad.PreRender();
    blurShader->EnableAttributes();
    m_data->quad.Render();
    blurShader->ClearTexture(0, false, true);

    // Render second vertical blur pass
    m_data->blurTargetP2.SetActive();
    horizontalPass = 0.0f;
    verticalPass = 1.0f;
    blurShader->SendUniformFloat("verticalPass", &verticalPass, 1);
    blurShader->SendUniformFloat("horizontalPass", &horizontalPass, 1);

    blurShader->SendTexture(0, m_data->blurTargetP1.GetTextureID(), false, true);
    m_data->quad.PreRender();
    blurShader->EnableAttributes();
    m_data->quad.Render();
    blurShader->ClearTexture(0, false, true);
}

void OpenglEngine::RenderPostProcessing(const PostProcessing& post)
{
    auto& postShader = m_data->shaders[POST_SHADER_INDEX];
    postShader->SetActive();
    m_data->backBuffer.SetActive();

    postShader->SendUniformFloat("fadeAmount", &m_data->fadeAmount, 1);
    postShader->SendUniformFloat("glowAmount", &post.glowAmount, 1);
    postShader->SendUniformFloat("contrast", &post.contrast, 1);
    postShader->SendUniformFloat("saturation", &post.saturation, 1);
    postShader->SendUniformFloat("dofDistance", &post.dofDistance, 1);
    postShader->SendUniformFloat("dofFade", &post.dofFade, 1);
    postShader->SendUniformFloat("fogDistance", &post.fogDistance, 1);
    postShader->SendUniformFloat("fogFade", &post.fogFade, 1);
    postShader->SendUniformFloat("fogColor", &post.fogColour.r, 3);
    postShader->SendUniformFloat("minimumColor", &post.minimumColour.r, 3);
    postShader->SendUniformFloat("maximumColor", &post.maximumColour.r, 3);

    postShader->SendUniformFloat("finalMask", &post.masks[PostProcessing::FINAL_MAP], 1);
    postShader->SendUniformFloat("sceneMask", &post.masks[PostProcessing::SCENE_MAP], 1);
    postShader->SendUniformFloat("normalMask", &post.masks[PostProcessing::NORMAL_MAP], 1);
    postShader->SendUniformFloat("depthMask", &post.masks[PostProcessing::DEPTH_MAP], 1);
    postShader->SendUniformFloat("glowMask", &post.masks[PostProcessing::GLOW_MAP], 1);
    postShader->SendUniformFloat("blurGlowMask", &post.masks[PostProcessing::BLUR_GLOW_MAP], 1);
    postShader->SendUniformFloat("blurSceneMask", &post.masks[PostProcessing::BLUR_SCENE_MAP], 1);
    postShader->SendUniformFloat("depthOfFieldMask", &post.masks[PostProcessing::DOF_MAP], 1);
    postShader->SendUniformFloat("fogMask", &post.masks[PostProcessing::FOG_MAP], 1);

    postShader->SendTexture(PostProcessing::SCENE, m_data->sceneTarget.GetTextureID(), false, true);
    postShader->SendTexture(PostProcessing::NORMAL, m_data->normalTarget.GetTextureID(), false, true);
    postShader->SendTexture(PostProcessing::BLUR, m_data->blurTargetP2.GetTextureID(), false, true);

    m_data->quad.PreRender();
    postShader->EnableAttributes();
    m_data->quad.Render();

    postShader->ClearTexture(PostProcessing::SCENE, false, true);
    postShader->ClearTexture(PostProcessing::NORMAL, false, true);
    postShader->ClearTexture(PostProcessing::BLUR, false, true);
}

void OpenglEngine::SetTextures(const std::vector<int>& textureIDs)
{
    auto& shader = m_data->shaders[m_data->selectedShader];

    int slot = 0;
    for(int id : textureIDs)
    {
        if(id != NO_INDEX && shader->HasTextureSlot(slot))
        {
            const auto& texture = m_data->textures[id];
            shader->SendTexture(slot, texture->GetID(), texture->IsCubeMap(), false);
            ++slot;
        }
    }
}

void OpenglEngine::SetSelectedShader(int index)
{
    m_data->selectedShader = index;
    m_data->shaders[index]->SetActive();
}

void OpenglEngine::UpdateShader(const Mesh& mesh, 
                                const PostProcessing& post)
{
    const int index = mesh.normalIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        SetSelectedShader(index);

        shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        shader->SendUniformFloat("depthNear", &post.depthNear, 1);
        shader->SendUniformFloat("depthFar", &post.depthFar, 1);
    }

    shader->SendUniformFloat("meshBump", &mesh.bump, 1);
}

void OpenglEngine::UpdateShader(const Mesh& mesh, 
                                const std::vector<Light>& lights)
{
    const int index = mesh.shaderIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        SetSelectedShader(index);

        shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        shader->SendUniformFloat("cameraPosition", &m_data->cameraPosition.x, 3);
        shader->SendLights(lights);
        shader->SendUniformArrays();
    }

    shader->SendUniformFloat("meshAmbience", &mesh.ambience, 1);
    shader->SendUniformFloat("meshBump", &mesh.bump, 1);
    shader->SendUniformFloat("meshGlow", &mesh.glow, 1);
    shader->SendUniformFloat("meshSpecularity", &mesh.specularity, 1);
}

void OpenglEngine::UpdateShader(const Water& water, 
                                const PostProcessing& post)
{
    const int index = water.normalIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        SetSelectedShader(index);

        shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        shader->SendUniformFloat("depthNear", &post.depthNear, 1);
        shader->SendUniformFloat("depthFar", &post.depthFar, 1);
    }
}

void OpenglEngine::UpdateShader(const Water& water, 
                                const std::vector<Light>& lights,
                                float timer)
{
    const int index = water.shaderIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        SetSelectedShader(index);

        shader->SendUniformFloat("timer", &timer, 1);
        shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        shader->SendUniformFloat("cameraPosition", &m_data->cameraPosition.x, 3);
        shader->SendLights(lights);
    }

    shader->SendUniformFloat("speed", &water.speed, 1);
    shader->SendUniformFloat("bumpIntensity", &water.bump, 1);
    shader->SendUniformFloat("bumpVelocity", &water.bumpVelocity.x, 2);
    shader->SendUniformFloat("uvScale", &water.uvScale.x, 2);
    shader->SendUniformFloat("deepColor", &water.deepColour.r, 4);
    shader->SendUniformFloat("shallowColor", &water.shallowColour.r, 4);
    shader->SendUniformFloat("reflectionTint", &water.reflectionTint.r, 3);
    shader->SendUniformFloat("reflectionIntensity", &water.reflection, 1);
    shader->SendUniformFloat("fresnal", &water.fresnal.x, 3);

    for (unsigned int i = 0; i < water.waves.size(); ++i)
    {
        shader->UpdateUniformArray("waveFrequency", &water.waves[i].amplitude, 1, i);
        shader->UpdateUniformArray("waveAmplitude", &water.waves[i].amplitude, 1, i);
        shader->UpdateUniformArray("wavePhase", &water.waves[i].phase, 1, i);
        shader->UpdateUniformArray("waveDirectionX", &water.waves[i].directionX, 1, i);
        shader->UpdateUniformArray("waveDirectionZ", &water.waves[i].directionZ, 1, i);
    }

    shader->SendUniformArrays();
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
    return m_data->shaders[index]->GetText();
}

std::string OpenglEngine::GetShaderAssembly(int index)
{
    return m_data->shaders[index]->GetAssembly();
}

void OpenglEngine::SetFade(float value)
{
    m_data->fadeAmount = value;
}

void OpenglEngine::WriteToShader(const std::string& name,
                                 const std::string& text)
{
    auto WriteToFile = [](const std::string& contents, const std::string& filepath)
    {
        std::ofstream file(filepath.c_str(), std::ios_base::out | std::ios_base::trunc);
        if (!file.is_open())
        {
            Logger::LogError("Could not open " + filepath);
        }
        else
        {
            file << contents << std::endl;
            file.close();
        }
    };

    // GLSL uses two files that both must start with GLSL_HEADER
    // Note first component in split regex vector is whitespace or empty

    std::vector<std::string> components;
    boost::algorithm::split_regex(components, text, boost::regex(GLSL_HEADER));

    WriteToFile(GLSL_HEADER + components[1], 
        GENERATED_PATH + name + GLSL_VERTEX_EXTENSION);

    WriteToFile(GLSL_HEADER + components[2], 
        GENERATED_PATH + name + GLSL_FRAGMENT_EXTENSION);
}

void OpenglEngine::EnableAlphaBlending(bool enable)
{
    enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}