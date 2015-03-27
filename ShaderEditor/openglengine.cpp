////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglengine.h"
#include "openglshader.h"
#include "openglmesh.h"
#include "opengltexture.h"
#include "opengltarget.h"
#include "openglemitter.h"
#include "sceneInterface.h"
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

    HGLRC hrc = nullptr;                 ///< Rendering context  
    HDC hdc = nullptr;                   ///< Device context                            
    GlRenderTarget backBuffer;           ///< Render target for the back buffer
    GlRenderTarget sceneTarget;          ///< Render target for the main scene
    GlRenderTarget preEffectsTarget;     ///< Render target for pre-rendering effects
    GlRenderTarget blurTarget;           ///< Render target for blurring the scene
    GlQuad quad;                         ///< Quad to render the final post processed scene onto
    glm::vec3 cameraPosition;            ///< Position of the camera
    glm::vec3 cameraUp;                  ///< The up vector of the camera
    glm::mat4 projection;                ///< Projection matrix
    glm::mat4 view;                      ///< View matrix
    glm::mat4 viewProjection;            ///< View projection matrix
    bool isBackfaceCull = false;         ///< Whether the culling rasterize state is active
    bool isAlphaBlend = false;           ///< Whether alpha blending is currently active
    bool isDepthWrite = false;           ///< Whether writing to the depth buffer is active
    bool isWireframe = false;            ///< Whether to render the scene as wireframe
    bool useDiffuseTextures = true;      ///< Whether to render diffuse textures
    int selectedShader = NO_INDEX;       ///< Currently active shader for rendering
    float fadeAmount = 0.0f;             ///< the amount to fade the scene by
                                        
    std::vector<std::unique_ptr<GlTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<GlMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<GlWater>> waters;     ///< Each water in the scene
    std::vector<std::unique_ptr<GlTerrain>> terrain;     ///< Each terrain in the scene
    std::vector<std::unique_ptr<GlShader>> shaders;   ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<GlEmitter>> emitters; ///< Emitters holding particles
};

OpenglData::OpenglData() :
    quad("ScreenQuad"),
    sceneTarget("SceneTarget", SCENE_TEXTURES, true),
    preEffectsTarget("PreEffectsTarget", EFFECTS_TEXTURES, false),
    blurTarget("BlurTarget", BLUR_TEXTURES, false, true),
    backBuffer("BackBuffer")
{
}

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
    selectedShader = NO_INDEX;
    fadeAmount = 0.0f;

    for(auto& texture : textures)
    {
        texture->Release();
    }

    for(auto& mesh : meshes)
    {
        mesh->Release();
    }

    for(auto& mesh : terrain)
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

    for(auto& emitter : emitters)
    {
        emitter->Release();
    }

    backBuffer.Release();
    sceneTarget.Release();
    preEffectsTarget.Release();
    blurTarget.Release();
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
       !m_data->preEffectsTarget.Initialise() ||
       !m_data->blurTarget.Initialise())
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

    // Initialise the opengl environment
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 

    m_data->isBackfaceCull = false;
    m_data->isAlphaBlend = true;
    m_data->isDepthWrite = false;
    m_data->isWireframe = false;
    EnableBackfaceCull(true);
    EnableAlphaBlending(false);
    EnableDepthWrite(true);

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

bool OpenglEngine::InitialiseScene(const IScene& scene)
{
    m_data->textures.reserve(scene.Textures().size());
    for(const auto& texture : scene.Textures())
    {
        m_data->textures.push_back(std::unique_ptr<GlTexture>(
            new GlTexture(*texture)));
    }

    m_data->shaders.reserve(scene.Shaders().size());
    for(const auto& shader : scene.Shaders())
    {
        m_data->shaders.push_back(std::unique_ptr<GlShader>(
            new GlShader(*shader)));
    }

    m_data->meshes.reserve(scene.Meshes().size());
    for(const auto& mesh : scene.Meshes())
    {
        m_data->meshes.push_back(std::unique_ptr<GlMesh>(new GlMesh(*mesh,
            [this](const glm::mat4& world, const Colour& colour){ UpdateShader(world, colour); })));
    }

    m_data->terrain.reserve(scene.Terrains().size());
    for(const auto& terrain : scene.Terrains())
    {
        m_data->terrain.push_back(std::unique_ptr<GlTerrain>(new GlTerrain(*terrain)));
    }

    m_data->waters.reserve(scene.Waters().size());
    for(const auto& water : scene.Waters())
    {
        m_data->waters.push_back(std::unique_ptr<GlWater>(new GlWater(*water)));
    }

    m_data->emitters.reserve(scene.Emitters().size());
    for(const auto& emitter : scene.Emitters())
    {
        m_data->emitters.push_back(std::unique_ptr<GlEmitter>(new GlEmitter(*emitter,
            [this](const glm::mat4& world, const Particle& data){ UpdateShader(world, data); })));
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

    for(auto& terrain : m_data->terrain)
    {
        if(!terrain->Initialise())
        {
            Logger::LogError("OpenGL: Failed to re-initialise terrain");
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

    for(auto& emitter : m_data->emitters)
    {
        if(!emitter->Initialise())
        {
            Logger::LogError("OpenGL: Failed to re-initialise emitter");
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

void OpenglEngine::Render(const IScene& scene, float timer)
{
    RenderSceneMap(scene, timer);
    RenderPreEffects(scene.Post());
    RenderBlur(scene.Post());
    RenderPostProcessing(scene.Post());
    SwapBuffers(m_data->hdc); 
}

void OpenglEngine::RenderSceneMap(const IScene& scene, float timer)
{
    m_data->sceneTarget.SetActive();

    if (m_data->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    }

    RenderTerrain(scene);
    RenderMeshes(scene);
    RenderWater(scene, timer);
    RenderEmitters();

    if (m_data->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    }
}

void OpenglEngine::RenderMeshes(const IScene& scene)
{
    for (auto& mesh : m_data->meshes)
    {
        if (UpdateShader(mesh->GetMesh(), scene))
        {
            mesh->PreRender();
            EnableAttributes();
            mesh->Render();
        }
    }
}

void OpenglEngine::RenderTerrain(const IScene& scene)
{
    for (auto& terrain : m_data->terrain)
    {
        if (UpdateShader(terrain->GetTerrain(), scene))
        {
            terrain->PreRender();
            EnableAttributes();
            terrain->Render();
        }
    }
}

void OpenglEngine::RenderWater(const IScene& scene, float timer)
{
    for (auto& water : m_data->waters)
    {
        if (UpdateShader(water->GetWater(), scene, timer))
        {
            water->PreRender();
            EnableAttributes();
            water->Render();
        }
    }
}

void OpenglEngine::RenderEmitters()
{
    EnableDepthWrite(false);

    for (auto& emitter : m_data->emitters)
    {
        if (UpdateShader(emitter->GetEmitter()))
        {
            emitter->PreRender();
            EnableAttributes();
            emitter->Render(m_data->cameraPosition, m_data->cameraUp);
        }
    }

    EnableDepthWrite(true);
}

void OpenglEngine::RenderPreEffects(const PostProcessing& post)
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false);

    SetSelectedShader(PRE_SHADER);
    auto& preShader = m_data->shaders[PRE_SHADER];
    preShader->SetActive();

    preShader->SendUniformFloat("normalMask", &post.Mask(PostProcessing::NORMAL_MAP), 1);
    preShader->SendUniformFloat("bloomStart", &post.BloomStart(), 1);
    preShader->SendUniformFloat("bloomFade", &post.BloomFade(), 1);

    preShader->SendTexture(0, m_data->sceneTarget, SCENE_ID);
    preShader->SendTexture(1, m_data->sceneTarget, NORMAL_ID);
    SendTexture(2, RANDOM_TEXTURE_ID);
    
    m_data->preEffectsTarget.SetActive();
    m_data->quad.PreRender();
    preShader->EnableAttributes();
    m_data->quad.Render();

    preShader->ClearTexture(0, m_data->sceneTarget);
    preShader->ClearTexture(1, m_data->sceneTarget);
}

void OpenglEngine::RenderBlur(const PostProcessing& post)
{
    EnableAlphaBlending(false);
    EnableBackfaceCull(false);

    m_data->blurTarget.SetActive();
    m_data->blurTarget.SwitchTextures();

    SetSelectedShader(BLUR_HORIZONTAL_SHADER);
    auto& blurHorizontal = m_data->shaders[BLUR_HORIZONTAL_SHADER];
    blurHorizontal->SetActive();
    blurHorizontal->SendUniformFloat("blurStep", &post.BlurStep(), 1);

    blurHorizontal->SendTexture(0, m_data->preEffectsTarget, SCENE_ID);

    m_data->quad.PreRender();
    blurHorizontal->EnableAttributes();
    m_data->quad.Render();

    blurHorizontal->ClearTexture(0, m_data->preEffectsTarget);

    SetSelectedShader(BLUR_VERTICAL_SHADER);
    auto& blurVertical = m_data->shaders[BLUR_VERTICAL_SHADER];
    blurVertical->SetActive();
    blurVertical->SendUniformFloat("blurStep", &post.BlurStep(), 1);

    blurVertical->SendTexture(0, m_data->blurTarget, BLUR_ID);
    m_data->blurTarget.SwitchTextures();

    m_data->quad.PreRender();
    blurVertical->EnableAttributes();
    m_data->quad.Render();

    blurVertical->ClearTexture(0, m_data->blurTarget);
}

void OpenglEngine::RenderPostProcessing(const PostProcessing& post)
{
    m_data->useDiffuseTextures = post.UseDiffuseTextures();

    EnableAlphaBlending(false);
    EnableBackfaceCull(false);

    SetSelectedShader(POST_SHADER);
    auto& postShader = m_data->shaders[POST_SHADER];
    postShader->SetActive();
    m_data->backBuffer.SetActive();

    postShader->SendUniformFloat("bloomIntensity", &post.BloomIntensity(), 1);
    postShader->SendUniformFloat("fadeAmount", &m_data->fadeAmount, 1);
    postShader->SendUniformFloat("contrast", &post.Contrast(), 1);
    postShader->SendUniformFloat("saturation", &post.Saturation(), 1);
    postShader->SendUniformFloat("dofStart", &post.DOFStart(), 1);
    postShader->SendUniformFloat("dofFade", &post.DOFFade(), 1);
    postShader->SendUniformFloat("fogStart", &post.FogStart(), 1);
    postShader->SendUniformFloat("fogFade", &post.FogFade(), 1);
    postShader->SendUniformFloat("fogColor", &post.FogColour().r, 3);
    postShader->SendUniformFloat("minimumColor", &post.MinColour().r, 3);
    postShader->SendUniformFloat("maximumColor", &post.MaxColour().r, 3);

    postShader->SendUniformFloat("finalMask", &post.Mask(PostProcessing::FINAL_MAP), 1);
    postShader->SendUniformFloat("sceneMask", &post.Mask(PostProcessing::SCENE_MAP), 1);
    postShader->SendUniformFloat("normalMask", &post.Mask(PostProcessing::NORMAL_MAP), 1);
    postShader->SendUniformFloat("depthMask", &post.Mask(PostProcessing::DEPTH_MAP), 1);
    postShader->SendUniformFloat("blurSceneMask", &post.Mask(PostProcessing::BLUR_MAP), 1);
    postShader->SendUniformFloat("depthOfFieldMask", &post.Mask(PostProcessing::DOF_MAP), 1);
    postShader->SendUniformFloat("fogMask", &post.Mask(PostProcessing::FOG_MAP), 1);
    postShader->SendUniformFloat("bloomMask", &post.Mask(PostProcessing::BLOOM_MAP), 1);
    postShader->SendUniformFloat("ambienceMask", &post.Mask(PostProcessing::AMBIENCE_MAP), 1);

    postShader->SendTexture(0, m_data->preEffectsTarget, SCENE_ID);
    postShader->SendTexture(1, m_data->preEffectsTarget, EFFECTS_ID);
    postShader->SendTexture(2, m_data->blurTarget, BLUR_ID);

    m_data->quad.PreRender();
    postShader->EnableAttributes();
    m_data->quad.Render();

    postShader->ClearTexture(0, m_data->preEffectsTarget);
    postShader->ClearTexture(1, m_data->preEffectsTarget);
    postShader->ClearTexture(2, m_data->blurTarget);
}

bool OpenglEngine::UpdateShader(const Emitter& emitter)
{
    const int index = emitter.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if (index != m_data->selectedShader)
        {
            SetSelectedShader(index);
        }

        shader->SendUniformFloat("tint", &emitter.Tint().r, 4);

        EnableBackfaceCull(false);
        EnableAlphaBlending(true);
        return true;
    }
    return false;
}

void OpenglEngine::UpdateShader(const glm::mat4& world, const Particle& particle)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->SendUniformMatrix("worldViewProjection", m_data->viewProjection * world);
    shader->SendUniformFloat("alpha", &particle.Alpha(), 1);
    SendTexture(0, particle.Texture());
}

void OpenglEngine::UpdateShader(const glm::mat4& world, const Colour& colour)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->SendUniformMatrix("world", world);
    shader->SendUniformFloat("meshColour", &colour.r, 3);
}

bool OpenglEngine::UpdateShader(const MeshData& mesh,
                                const IScene& scene,
                                bool alphaBlend,
                                float timer)
{
    const int index = mesh.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            SendLights(scene.Lights());
            shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
            shader->SendUniformFloat("cameraPosition", &m_data->cameraPosition.x, 3);
            shader->SendUniformFloat("depthNear", &scene.Post().DepthNear(), 1);
            shader->SendUniformFloat("depthFar", &scene.Post().DepthFar(), 1);

            if (timer >= 0.0f)
            {
                shader->SendUniformFloat("timer", &timer, 1);
            }
        }
    
        SendTextures(mesh.TextureIDs());
        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(alphaBlend);
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Terrain& terrain, const IScene& scene)
{
    if (UpdateShader(terrain, scene, false))
    {
        auto& shader = m_data->shaders[terrain.ShaderID()];
        shader->SendUniformFloat("meshCaustics", &terrain.Caustics(), 1);
        shader->SendUniformFloat("meshAmbience", &terrain.Ambience(), 1);
        shader->SendUniformFloat("meshBump", &terrain.Bump(), 1);
        shader->SendUniformFloat("meshSpecularity", &terrain.Specularity(), 1);
        shader->SendUniformArrays();
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Mesh& mesh, const IScene& scene)
{
    if (UpdateShader(mesh, scene, false))
    {
        auto& shader = m_data->shaders[mesh.ShaderID()];
        shader->SendUniformFloat("meshCaustics", &mesh.Caustics(), 1);
        shader->SendUniformFloat("meshAmbience", &mesh.Ambience(), 1);
        shader->SendUniformFloat("meshBump", &mesh.Bump(), 1);
        shader->SendUniformFloat("meshSpecularity", &mesh.Specularity(), 1);
        shader->SendUniformArrays();
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Water& water, const IScene& scene, float timer)
{
    if (UpdateShader(water, scene, true, timer))
    {
        auto& shader = m_data->shaders[water.ShaderID()];
        shader->SendUniformFloat("speed", &water.Speed(), 1);
        shader->SendUniformFloat("bumpIntensity", &water.Bump(), 1);
        shader->SendUniformFloat("bumpVelocity", &water.BumpVelocity().x, 2);
        shader->SendUniformFloat("uvScale", &water.UVScale().x, 2);
        shader->SendUniformFloat("deepColor", &water.Deep().r, 4);
        shader->SendUniformFloat("shallowColor", &water.Shallow().r, 4);
        shader->SendUniformFloat("reflectionTint", &water.ReflectionTint().r, 3);
        shader->SendUniformFloat("reflectionIntensity", &water.ReflectionIntensity(), 1);
        shader->SendUniformFloat("fresnal", &water.Fresnal().x, 3);

        const auto& waves = water.Waves();
        for (unsigned int i = 0; i < waves.size(); ++i)
        {
            shader->UpdateUniformArray("waveFrequency", &waves[i].amplitude, 1, i);
            shader->UpdateUniformArray("waveAmplitude", &waves[i].amplitude, 1, i);
            shader->UpdateUniformArray("wavePhase", &waves[i].phase, 1, i);
            shader->UpdateUniformArray("waveDirectionX", &waves[i].directionX, 1, i);
            shader->UpdateUniformArray("waveDirectionZ", &waves[i].directionZ, 1, i);
        }

        shader->SendUniformArrays();
        return true;
    }
    return false;
}

void OpenglEngine::SendLights(const std::vector<std::unique_ptr<Light>>& lights)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i*3; // Arrays pack tightly
        shader->UpdateUniformArray("lightSpecularity", &lights[i]->Specularity(), 1, i);
        shader->UpdateUniformArray("lightActive", &lights[i]->Active(), 1, i);
        shader->UpdateUniformArray("lightAttenuation", &lights[i]->Attenuation().x, 3, offset);
        shader->UpdateUniformArray("lightPosition", &lights[i]->Position().x, 3, offset);
        shader->UpdateUniformArray("lightDiffuse", &lights[i]->Diffuse().r, 3, offset);
        shader->UpdateUniformArray("lightSpecular", &lights[i]->Specular().r, 3, offset);
    }
}

void OpenglEngine::SendTextures(const std::vector<int>& textures)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0, slot = 0; i < textures.size(); ++i)
    {
        const Texture::Type type = static_cast<Texture::Type>(i);
        const bool isDiffuse = type == Texture::DIFFUSE;

        const int ID = (isDiffuse && !m_data->useDiffuseTextures) ?
            BLANK_TEXTURE_ID : textures[type];

        if (SendTexture(slot, ID))
        {
            ++slot;
        }
    }
}

bool OpenglEngine::SendTexture(int slot, int ID)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    if (ID != NO_INDEX && shader->HasTextureSlot(slot))
    {
        const auto& texture = m_data->textures[ID];
        shader->SendTexture(slot, texture->GetID(), texture->IsCubeMap());
        return true;
    }
    return false;
}

void OpenglEngine::SetSelectedShader(int index)
{
    m_data->selectedShader = index;
    m_data->shaders[index]->SetActive();
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

    m_data->cameraUp.x = world.m12;
    m_data->cameraUp.y = world.m22;
    m_data->cameraUp.z = world.m32;

    m_data->view = glm::inverse(view);
    m_data->viewProjection = m_data->projection * m_data->view;
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
    boost::algorithm::split_regex(components, 
        text, boost::regex(GlShader::GetShaderHeader()));

    WriteToFile(GlShader::GetShaderHeader() + components[1], 
        GENERATED_PATH + name + GLSL_VERTEX_EXTENSION);

    WriteToFile(GlShader::GetShaderHeader() + components[2], 
        GENERATED_PATH + name + GLSL_FRAGMENT_EXTENSION);
}

void OpenglEngine::EnableAlphaBlending(bool enable)
{
    if (enable != m_data->isAlphaBlend)
    {
        m_data->isAlphaBlend = enable;
        enable ? glEnablei(GL_BLEND, 0) : glDisablei(GL_BLEND, 0);
    }
}

void OpenglEngine::EnableBackfaceCull(bool enable)
{
    if(enable != m_data->isBackfaceCull)
    {
        m_data->isBackfaceCull = enable;
        enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
}

void OpenglEngine::EnableDepthWrite(bool enable)
{
    if (enable != m_data->isDepthWrite)
    {
        m_data->isDepthWrite = enable;
        enable ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
    }
}

void OpenglEngine::EnableAttributes()
{
    m_data->shaders[m_data->selectedShader]->EnableAttributes();
}

void OpenglEngine::ToggleWireframe()
{
    m_data->isWireframe = !m_data->isWireframe;
}