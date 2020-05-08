////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_engine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl_engine.h"
#include "opengl_common.h"
#include "opengl_shader.h"
#include "opengl_mesh.h"
#include "opengl_texture.h"
#include "opengl_target.h"
#include "opengl_emitter.h"
#include "scene_interface.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

#include <array>
#include <fstream>
#include <sstream>

/**
* Internal data for the opengl rendering engine
*/
struct OpenglData
{
    OpenglData();
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
    bool isBlendMultiply = false;        ///< Whether to multiply the blend colours
    bool isDepthWrite = false;           ///< Whether writing to the depth buffer is active
    bool isWireframe = false;            ///< Whether to render the scene as wireframe
    bool useDiffuseTextures = true;      ///< Whether to render diffuse textures
    int selectedShader = -1;             ///< Currently active shader for rendering
    float fadeAmount = 0.0f;             ///< the amount to fade the scene by
                              
    std::unique_ptr<GlQuadMesh> shadows;              ///< Shadow instances
    std::vector<std::unique_ptr<GlTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<GlMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<GlMesh>> waters;      ///< Each water in the scene
    std::vector<std::unique_ptr<GlMesh>> terrain;     ///< Each terrain in the scene
    std::vector<std::unique_ptr<GlShader>> shaders;   ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<GlEmitter>> emitters; ///< Emitters holding particles
};

OpenglData::OpenglData()
    : quad("ScreenQuad")
    , sceneTarget("SceneTarget", SCENE_TEXTURES, true)
    , preEffectsTarget("PreEffectsTarget", EFFECTS_TEXTURES, false)
    , blurTarget("BlurTarget", BLUR_TEXTURES, false, true)
    , backBuffer("BackBuffer")
{
}

OpenglData::~OpenglData()
{
    Release();
}

void OpenglData::Release()
{
    selectedShader = -1;
    fadeAmount = 0.0f;

    if (shadows)
    {
        shadows->Release();
    }

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

OpenglEngine::OpenglEngine(HWND hwnd) :
    m_data(new OpenglData()),
    m_hwnd(hwnd)
{
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
    m_data->hdc = GetDC(m_hwnd); // Get device context for window

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_data->hdc, &pfd);
    if (pixelFormat == 0)
    {
        Logger::LogError("OpenGL: Pixel Format unsupported");
        return false;
    }

    if (!SetPixelFormat(m_data->hdc, pixelFormat, &pfd))
    {
        Logger::LogError("OpenGL: Set Pixel Format failed");
        return false;
    }

    // Create a temporary OpenGL 2.1 context for Glew
    HGLRC tempOpenGLContext = wglCreateContext(m_data->hdc);
    wglMakeCurrent(m_data->hdc, tempOpenGLContext);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Temporary context creation failed");
        return false;
    }

    if (glewInit() != GLEW_OK || !wglewIsSupported("WGL_ARB_create_context") || HasCallFailed())
    {
        Logger::LogError("OpenGL: GLEW Initialization failed");
        return false;
    }

    int attributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB,
        0, 0
    };

    // Create the actual opengl 3.0+ context
    m_data->hrc = wglCreateContextAttribsARB(m_data->hdc, 0, attributes);  
    wglMakeCurrent(0, 0);
    wglDeleteContext(tempOpenGLContext); 
    wglMakeCurrent(m_data->hdc, m_data->hrc);

    int minor, major;
    glGetIntegerv(GL_MAJOR_VERSION, &major); 
    glGetIntegerv(GL_MINOR_VERSION, &minor); 
    if(minor == -1 || major == -1)
    {
        Logger::LogError("OpenGL: Version not supported");
        return false;
    }

    // Create the render targets
    m_data->sceneTarget.SetHighQuality(DEPTH_ID); // Required for DOF
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
    glClearColor(0.22f, 0.49f, 0.85f, 0.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 

    m_data->isBackfaceCull = false;
    m_data->isAlphaBlend = true;
    m_data->isBlendMultiply = true;
    m_data->isDepthWrite = false;
    m_data->isWireframe = false;
    EnableBackfaceCull(true);
    EnableAlphaBlending(false, false);
    EnableDepthWrite(true);

    m_data->projection = glm::perspective(FIELD_OF_VIEW, 
        RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);

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
    m_data->shadows = std::make_unique<GlQuadMesh>(scene.Shadows(),
        [this](const glm::mat4& world, int texture){ UpdateShader(world, texture); });

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
            [this](const glm::mat4& world, int texture){ UpdateShader(world, texture); })));
    }

    m_data->terrain.reserve(scene.Terrains().size());
    for(const auto& terrain : scene.Terrains())
    {
        m_data->terrain.push_back(std::unique_ptr<GlMesh>(new GlMesh(*terrain,
            [this](const glm::mat4& world, int texture){ UpdateShader(world, texture); })));
    }

    m_data->waters.reserve(scene.Waters().size());
    for(const auto& water : scene.Waters())
    {
        m_data->waters.push_back(std::unique_ptr<GlMesh>(new GlMesh(*water,
            [this](const glm::mat4& world, int texture){ UpdateShader(world, texture); })));
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

    if (!m_data->shadows->Initialise())
    {
        Logger::LogError("OpenGL: Failed to re-initialise shadows");
        return false;
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
    RenderShadows();
    RenderMeshes(scene);
    RenderWater(scene, timer);
    RenderEmitters(scene);

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

void OpenglEngine::RenderShadows()
{
    if (UpdateShader(m_data->shadows->GetData()))
    {
        EnableDepthWrite(false);

        m_data->shadows->PreRender();
        EnableAttributes();
        m_data->shadows->Render();

        EnableDepthWrite(true);
    }
}

void OpenglEngine::RenderEmitters(const IScene& scene)
{
    EnableDepthWrite(false);

    for (auto& emitter : m_data->emitters)
    {
        if (UpdateShader(emitter->GetEmitter(), scene))
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
    EnableAlphaBlending(false, false);

    SetSelectedShader(PRE_SHADER);
    auto& preShader = m_data->shaders[PRE_SHADER];

    preShader->SendUniformFloat("bloomStart", &post.BloomStart(), 1);
    preShader->SendUniformFloat("bloomFade", &post.BloomFade(), 1);

    preShader->SendTexture("SceneSampler", m_data->sceneTarget, SCENE_ID);
    
    m_data->preEffectsTarget.SetActive();
    m_data->quad.PreRender();
    preShader->EnableAttributes();
    m_data->quad.Render();

    preShader->ClearTexture("SceneSampler", m_data->sceneTarget);
}

void OpenglEngine::RenderBlur(const PostProcessing& post)
{
    EnableAlphaBlending(false, false);
    EnableBackfaceCull(false);

    m_data->blurTarget.SetActive();
    m_data->blurTarget.SwitchTextures();

    SetSelectedShader(BLUR_HORIZONTAL_SHADER);
    auto& blurHorizontal = m_data->shaders[BLUR_HORIZONTAL_SHADER];

    blurHorizontal->SendUniformFloat("blurStep", &post.BlurStep(), 1);
    blurHorizontal->SendTexture("SceneSampler", m_data->preEffectsTarget, SCENE_ID);

    m_data->quad.PreRender();
    blurHorizontal->EnableAttributes();
    m_data->quad.Render();

    blurHorizontal->ClearTexture("SceneSampler", m_data->preEffectsTarget);

    SetSelectedShader(BLUR_VERTICAL_SHADER);
    auto& blurVertical = m_data->shaders[BLUR_VERTICAL_SHADER];
    
    blurVertical->SendUniformFloat("blurStep", &post.BlurStep(), 1);
    blurVertical->SendTexture("SceneSampler", m_data->blurTarget, BLUR_ID);

    m_data->blurTarget.SwitchTextures();

    m_data->quad.PreRender();
    blurVertical->EnableAttributes();
    m_data->quad.Render();

    blurVertical->ClearTexture("SceneSampler", m_data->blurTarget);
}

void OpenglEngine::RenderPostProcessing(const PostProcessing& post)
{
    m_data->useDiffuseTextures = post.UseDiffuseTextures();

    EnableAlphaBlending(false, false);
    EnableBackfaceCull(false);

    m_data->backBuffer.SetActive();

    SetSelectedShader(POST_SHADER);
    auto& postShader = m_data->shaders[POST_SHADER];

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
    postShader->SendUniformFloat("depthMask", &post.Mask(PostProcessing::DEPTH_MAP), 1);
    postShader->SendUniformFloat("blurSceneMask", &post.Mask(PostProcessing::BLUR_MAP), 1);
    postShader->SendUniformFloat("depthOfFieldMask", &post.Mask(PostProcessing::DOF_MAP), 1);
    postShader->SendUniformFloat("fogMask", &post.Mask(PostProcessing::FOG_MAP), 1);
    postShader->SendUniformFloat("bloomMask", &post.Mask(PostProcessing::BLOOM_MAP), 1);

    postShader->SendTexture("SceneSampler", m_data->preEffectsTarget, SCENE_ID);
    postShader->SendTexture("BlurSampler", m_data->blurTarget, BLUR_ID);
    postShader->SendTexture("DepthSampler", m_data->sceneTarget, DEPTH_ID);

    m_data->quad.PreRender();
    postShader->EnableAttributes();
    m_data->quad.Render();

    postShader->ClearTexture("SceneSampler", m_data->preEffectsTarget);
    postShader->ClearTexture("BlurSampler", m_data->blurTarget);
    postShader->ClearTexture("DepthSampler", m_data->sceneTarget);
}

bool OpenglEngine::UpdateShader(const MeshData& quad)
{
    const int index = quad.ShaderID();
    if (index != -1)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->SendUniformMatrix("viewProjection", m_data->viewProjection);
        }

        EnableBackfaceCull(false);
        EnableAlphaBlending(true, true);
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Emitter& emitter, const IScene& scene)
{
    const int index = emitter.ShaderID();
    if (index != -1)
    {
        auto& shader = m_data->shaders[index];
        if (index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->SendUniformFloat("depthNear", &scene.Post().DepthNear(), 1);
            shader->SendUniformFloat("depthFar", &scene.Post().DepthFar(), 1);
        }

        shader->SendUniformFloat("tint", &emitter.Tint().r, 4);

        EnableBackfaceCull(false);
        EnableAlphaBlending(true, false);
        return true;
    }
    return false;
}

void OpenglEngine::UpdateShader(const glm::mat4& world, const Particle& particle)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->SendUniformMatrix("worldViewProjection", m_data->viewProjection * world);
    shader->SendUniformFloat("alpha", &particle.Alpha(), 1);
    SendTexture("DiffuseSampler", particle.Texture());
}

void OpenglEngine::UpdateShader(const glm::mat4& world, int texture)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->SendUniformMatrix("world", world);
    SendTexture("DiffuseSampler", m_data->useDiffuseTextures ? texture : BLANK_TEXTURE_ID);
}

bool OpenglEngine::UpdateShader(const MeshData& mesh,
                                const IScene& scene,
                                bool alphaBlend,
                                float timer)
{
    const int index = mesh.ShaderID();
    if (index != -1)
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

            if (index == WATER_SHADER)
            {
                shader->SendUniformFloat("timer", &timer, 1);
            }
        }
    
        SendTextures(mesh.TextureIDs());
        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(alphaBlend, false);
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Terrain& terrain, const IScene& scene)
{
    if (UpdateShader(terrain, scene, false))
    {
        SendAttributes(terrain);
        m_data->shaders[m_data->selectedShader]->SendUniformArrays();
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Mesh& mesh, const IScene& scene)
{
    if (UpdateShader(mesh, scene, false))
    {
        SendAttributes(mesh);
        m_data->shaders[m_data->selectedShader]->SendUniformArrays();
        return true;
    }
    return false;
}

bool OpenglEngine::UpdateShader(const Water& water, 
                                const IScene& scene, 
                                float timer)
{
    if (UpdateShader(water, scene, true, timer))
    {
        auto& shader = m_data->shaders[water.ShaderID()];
        shader->SendUniformFloat("speed", &water.Speed(), 1);
        shader->SendUniformFloat("bumpIntensity", &water.Bump(), 1);
        shader->SendUniformFloat("bumpScale", &water.BumpScale().x, 2);
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
            shader->UpdateUniformArray("waveAmplitude", &waves[i].frequency, 1, i);
            shader->UpdateUniformArray("wavePhase", &waves[i].phase, 1, i);
            shader->UpdateUniformArray("waveDirectionX", &waves[i].directionX, 1, i);
            shader->UpdateUniformArray("waveDirectionZ", &waves[i].directionZ, 1, i);
        }

        shader->SendUniformArrays();
        return true;
    }
    return false;
}

void OpenglEngine::SendAttributes(const MeshAttributes& attributes)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->SendUniformFloat("meshCausticAmount", &attributes.CausticsAmount(), 1);
    shader->SendUniformFloat("meshCausticScale", &attributes.CausticsScale(), 1);
    shader->SendUniformFloat("meshAmbience", &attributes.Ambience(), 1);
    shader->SendUniformFloat("meshBump", &attributes.Bump(), 1);
    shader->SendUniformFloat("meshSpecularity", &attributes.Specularity(), 1);
    shader->SendUniformFloat("meshSpecular", &attributes.Specular(), 1);
    shader->SendUniformFloat("meshDiffuse", &attributes.Diffuse(), 1);
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
    SendTexture("NormalSampler", textures[SLOT_NORMAL]);
    SendTexture("SpecularSampler", textures[SLOT_SPECULAR]);
    SendTexture("EnvironmentSampler", textures[SLOT_ENVIRONMENT]);
    SendTexture("CausticsSampler", textures[SLOT_CAUSTICS]);
}

bool OpenglEngine::SendTexture(const std::string& sampler, int ID)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    if (ID != -1)
    {
        const auto& texture = m_data->textures[ID];
        shader->SendTexture(sampler, texture->GetID(), texture->IsCubeMap());
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

void OpenglEngine::WriteToShader(const Shader& shader,
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

    WriteToFile(GlShader::GetShaderHeader() + components[1], shader.GLSLVertexFile());
    WriteToFile(GlShader::GetShaderHeader() + components[2], shader.GLSLFragmentFile());
}

void OpenglEngine::EnableAlphaBlending(bool enable, bool multiply)
{
    if (enable != m_data->isAlphaBlend)
    {
        m_data->isAlphaBlend = enable;
        for (int i = 0; i < MAX_TARGETS; ++i)
        {
            enable ? glEnablei(GL_BLEND, i) : glDisablei(GL_BLEND, i);
        }
    }
    if (multiply != m_data->isBlendMultiply)
    {
        m_data->isBlendMultiply = multiply;
        if (multiply)
        {
            glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_DST_ALPHA, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
        else
        {
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
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

void OpenglEngine::ReloadTexture(int index)
{
    const auto& name = m_data->textures[index]->Name();
    m_data->textures[index]->ReloadPixels() ?
        Logger::LogInfo("Texture: " + name + " reload successful") :
        Logger::LogError("Texture: " + name + " reload failed");
}

void OpenglEngine::ReloadTerrain(int index)
{
    const auto& name = m_data->terrain[index]->GetTerrain().Name();
    if (!m_data->terrain[index]->Reload())
    {
        Logger::LogError("Terrain: " + name + " reload failed");
    }
}