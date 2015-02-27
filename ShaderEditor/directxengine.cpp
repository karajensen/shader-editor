////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxcommon.h"
#include "directxshader.h"
#include "directxmesh.h"
#include "directxtexture.h"
#include "directxemitter.h"
#include "directxtarget.h"
#include "sceneInterface.h"
#include <array>
#include <fstream>

/**
* Internal data for the directx rendering engine
*/
struct DirectxData
{
    /**
    * Constructor
    */
    DirectxData();

    /**
    * Destructor
    */
    ~DirectxData();

    /**
    * Releases the device/context
    */
    void Release();

    ID3D11BlendState* alphaBlendState = nullptr;     ///< State for alpha blending
    ID3D11BlendState* noBlendState = nullptr;        ///< State for no alpha blending
    ID3D11RasterizerState* cullState = nullptr;      ///< Normal state of the rasterizer
    ID3D11RasterizerState* noCullState = nullptr;    ///< No face culling state of the rasterizer
    ID3D11DepthStencilState* writeState = nullptr;   ///< State for writing to the depth buffer
    ID3D11DepthStencilState* noWriteState = nullptr; ///< State for not writing to the depth buffer
    IDXGISwapChain* swapchain = nullptr;             ///< Collection of buffers for displaying frames
    ID3D11Device* device = nullptr;                  ///< Direct3D device interface
    ID3D11DeviceContext* context = nullptr;          ///< Direct3D device context
    ID3D11Debug* debug = nullptr;                    ///< Direct3D debug interface, only created in debug

    DxQuad quad;                        ///< Quad to render the final post processed scene onto
    DxRenderTarget backBuffer;          ///< Render target for the back buffer
    DxRenderTarget sceneTarget;         ///< Render target for the main scene
    DxRenderTarget normalTarget;        ///< Render target for the scene normal/depth map
    DxRenderTarget blurTargetP1;        ///< Render target for blurring the main scene pass1
    DxRenderTarget blurTargetP2;        ///< Render target for blurring the main scene pass2
    D3DXMATRIX view;                    ///< View matrix
    D3DXMATRIX projection;              ///< Projection matrix
    D3DXMATRIX viewProjection;          ///< View projection matrix
    D3DXVECTOR3 cameraPosition;         ///< Position of the camera
    D3DXVECTOR3 cameraUp;               ///< Up vector of the camera
    bool isBackfaceCull = false;        ///< Whether the culling rasterize state is active
    bool isAlphaBlend = false;          ///< Whether alpha blending is currently active
    bool isDepthWrite = false;          ///< Whether writing to the depth buffer is active
    int selectedShader = NO_INDEX;      ///< currently selected shader for rendering the scene
    float fadeAmount = 0.0f;            ///< the amount to fade the scene by
    float blendFactor = 1.0f;           ///< Alpha blend modifier
    
    std::vector<std::unique_ptr<DxTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<DxMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<DxWater>> waters;     ///< Each water in the scene
    std::vector<std::unique_ptr<DxShader>> shaders;   ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<DxEmitter>> emitters; ///< Particle emitters
};

DirectxData::DirectxData() :
    sceneTarget("SceneTarget"),
    normalTarget("NormalTarget"),
    blurTargetP1("BlurTargetP1"),
    blurTargetP2("BlurTargetP2"),
    backBuffer("BackBuffer", true),
    quad("SceneQuad")
{
}

DirectxData::~DirectxData()
{
    Release();
}

void DirectxData::Release()
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

    for(auto& water : waters)
    {
        water->Release();
    }

    for(auto& shader : shaders)
    {
        shader->Release();
    }

    for (auto& emitter : emitters)
    {
        emitter->Release();
    }

    quad.Release();
    sceneTarget.Release();
    normalTarget.Release();
    blurTargetP1.Release();
    blurTargetP2.Release();
    backBuffer.Release();

    SafeRelease(&noBlendState);
    SafeRelease(&alphaBlendState);
    SafeRelease(&cullState);
    SafeRelease(&noCullState);
    SafeRelease(&writeState);
    SafeRelease(&noWriteState);
    SafeRelease(&swapchain);
    SafeRelease(&context);
    SafeRelease(&device);

    if(debug)
    {
        debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        debug->Release();
        debug = nullptr;

        std::string seperator(100, '=');
        OutputDebugString((seperator + "\n").c_str());
    }
}

DirectxEngine::DirectxEngine(HWND hwnd) :
    m_data(new DirectxData()),
    m_hwnd(hwnd)
{
}

DirectxEngine::~DirectxEngine()
{
    Release();
}

void DirectxEngine::Release()
{
    m_data->Release();
}

bool DirectxEngine::Initialize()
{
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    scd.BufferCount = 1;                                
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    scd.OutputWindow = m_hwnd;                     
    scd.SampleDesc.Count = MULTISAMPLING_COUNT;                           
    scd.Windowed = TRUE; 
    scd.BufferDesc.Width = WINDOW_WIDTH;
    scd.BufferDesc.Height = WINDOW_HEIGHT;

    #ifdef _DEBUG
    unsigned int deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
    #else
    unsigned int deviceFlags = 0;
    #endif

    if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &scd, 
        &m_data->swapchain, &m_data->device, nullptr, &m_data->context)))
    {
        Logger::LogError("DirectX: Device creation failed");
        return false;
    }

    InitialiseDebugging();

    // Create the render targets. Back buffer must be initialised first.
    if(!m_data->backBuffer.Initialise(m_data->device, m_data->swapchain) ||
       !m_data->sceneTarget.Initialise(m_data->device) ||
       !m_data->normalTarget.Initialise(m_data->device) ||
       !m_data->blurTargetP1.Initialise(m_data->device) ||
       !m_data->blurTargetP2.Initialise(m_data->device))
    {
        Logger::LogError("DirectX: Failed to create render targets");
        return false;
    }

    // Create the post processing quad
    m_data->quad.Initialise(m_data->device, m_data->context);

	// Create the Blending states
    // Use zero for src alpha to prevent from interferring with glow
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; 
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(m_data->device->CreateBlendState(&blendDesc, &m_data->alphaBlendState)))
    {
        Logger::LogError("DirectX: Failed to create alpha blending state");
        return false;
    }

    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; 
    if (FAILED(m_data->device->CreateBlendState(&blendDesc, &m_data->noBlendState)))
    {
        Logger::LogError("DirectX: Failed to create no blending state");
        return false;
    }

    // Create the rasterizer state
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = true;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    rasterDesc.CullMode = D3D11_CULL_FRONT; // for Maya vert winding order
    if(FAILED(m_data->device->CreateRasterizerState(&rasterDesc, &m_data->cullState)))
    {
        Logger::LogError("DirectX: Failed to create cull rasterizer state");
        return false;
    }

    rasterDesc.CullMode = D3D11_CULL_NONE;
    if(FAILED(m_data->device->CreateRasterizerState(&rasterDesc, &m_data->noCullState)))
    {
        Logger::LogError("DirectX: Failed to create no cull rasterizer state");
        return false;
    }

    // Create the depth buffer state
    D3D11_DEPTH_STENCIL_DESC depthDesc;
    depthDesc.DepthEnable = true;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc.StencilEnable = false;

    if (FAILED(m_data->device->CreateDepthStencilState(&depthDesc, &m_data->writeState)))
    {
        Logger::LogError("DirectX: Failed to create write to depth buffer state");
        return false;
    }

    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    if (FAILED(m_data->device->CreateDepthStencilState(&depthDesc, &m_data->noWriteState)))
    {
        Logger::LogError("DirectX: Failed to create no write to depth buffer state");
        return false;
    }

    // Setup the directX environment
    m_data->isBackfaceCull = false;
    m_data->isAlphaBlend = true;
    m_data->isDepthWrite = false;
    EnableBackfaceCull(true);
    EnableAlphaBlending(false);
    EnableDepthWrite(true);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;
    viewport.MinDepth = 0.0;
    viewport.MaxDepth = 1.0;
    m_data->context->RSSetViewports(1, &viewport);

    D3DXMatrixPerspectiveFovLH(&m_data->projection,
        (FLOAT)D3DXToRadian(FIELD_OF_VIEW),
        (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 
        FRUSTRUM_NEAR, FRUSTRUM_FAR);

    SetDebugName(m_data->noBlendState, "NoBlendState");
    SetDebugName(m_data->alphaBlendState, "AlphaBlendState");
    SetDebugName(m_data->cullState, "CullState");
    SetDebugName(m_data->noCullState, "NoCullState");
    SetDebugName(m_data->writeState, "DepthWriteState");
    SetDebugName(m_data->noWriteState, "NoDepthWriteState");
    SetDebugName(m_data->device, "Device");
    SetDebugName(m_data->context, "Context");
    SetDebugName(m_data->swapchain, "SwapChain");

    Logger::LogInfo("DirectX: D3D11 sucessful");
    return true;
}

void DirectxEngine::InitialiseDebugging()
{
    #ifdef _DEBUG
    if(SUCCEEDED(m_data->device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_data->debug)))
    {
        ID3D11InfoQueue* infoQueue = nullptr;
        if(SUCCEEDED(m_data->debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)))
        {
            D3D11_MESSAGE_ID knownMessages[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
            };
 
            D3D11_INFO_QUEUE_FILTER filter;
            memset( &filter, 0, sizeof(filter) );
            filter.DenyList.NumIDs = _countof(knownMessages);
            filter.DenyList.pIDList = knownMessages;

            infoQueue->AddStorageFilterEntries(&filter);
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            infoQueue->Release();
        }
    }
    #endif
}

std::string DirectxEngine::CompileShader(int index)
{
    const std::string errors = m_data->shaders[index]->CompileShader(m_data->device);
    return errors.empty() ? "" : "\n\n" + errors;
}

bool DirectxEngine::InitialiseScene(const IScene& scene)
{
    m_data->textures.reserve(scene.Textures().size());
    for(const Texture& texture : scene.Textures())
    {
        m_data->textures.push_back(std::unique_ptr<DxTexture>(
            new DxTexture(texture.path)));
    }

    m_data->shaders.reserve(scene.Shaders().size());
    for(const Shader& shader : scene.Shaders())
    {
        m_data->shaders.push_back(std::unique_ptr<DxShader>(
            new DxShader(shader)));
    }

    m_data->meshes.reserve(scene.Meshes().size());
    for(const Mesh& mesh : scene.Meshes())
    {
        m_data->meshes.push_back(std::unique_ptr<DxMesh>(new DxMesh(mesh,
            [this](const D3DXMATRIX& world, const Colour& colour){ UpdateShader(world, colour); })));
    }

    m_data->waters.reserve(scene.Waters().size());
    for(const Water& water : scene.Waters())
    {
        m_data->waters.push_back(std::unique_ptr<DxWater>(new DxWater(water)));
    }

    m_data->emitters.reserve(scene.Emitters().size());
    for(const Emitter& emitter : scene.Emitters())
    {
        m_data->emitters.push_back(std::unique_ptr<DxEmitter>(new DxEmitter(emitter,
            [this](const D3DXMATRIX& world, const Particle& data){ UpdateShader(world, data); })));
    }

    return ReInitialiseScene();
}

bool DirectxEngine::ReInitialiseScene()
{
    for(unsigned int i = 0; i < m_data->shaders.size(); ++i)
    {
        const std::string result = CompileShader(i);
        if(!result.empty())
        {
            Logger::LogError("DirectX: " + 
                m_data->shaders[i]->GetName() + ": " + result);
            return false;
        }
    }

    for(auto& mesh : m_data->meshes)
    {
        mesh->Initialise(m_data->device, m_data->context);
    }

    for(auto& water : m_data->waters)
    {
        water->Initialise(m_data->device, m_data->context);
    }

    for(auto& texture : m_data->textures)
    {
        texture->Initialise(m_data->device);
    }

    for(auto& emitter : m_data->emitters)
    {
        emitter->Initialise(m_data->device, m_data->context);
    }

    Logger::LogInfo("DirectX: Re-Initialised");
    return true;
}

bool DirectxEngine::FadeView(bool in, float amount)
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

void DirectxEngine::Render(const IScene& scene, float timer)
{
    RenderSceneMap(scene.Lights(), timer);
    RenderNormalMap(scene.Post(), timer);
    RenderSceneBlur(scene.Post());
    RenderPostProcessing(scene.Post());
    m_data->swapchain->Present(0, 0);
}

void DirectxEngine::RenderSceneMap(const std::vector<Light>& lights, float timer)
{
    m_data->sceneTarget.SetActive(m_data->context);

    for (auto& mesh : m_data->meshes)
    {
        if (UpdateShader(mesh->GetMesh(), lights))
        {
            mesh->Render(m_data->context);
        }
    }

    for (auto& mesh : m_data->waters)
    {
        if (UpdateShader(mesh->GetWater(), lights, timer))
        {
            mesh->Render(m_data->context);
        }
    }
    
    RenderEmitters();
}

void DirectxEngine::RenderEmitters()
{
    EnableDepthWrite(false);

    for (auto& emitter : m_data->emitters)
    {
        if (UpdateShader(emitter->GetEmitter()))
        {
            emitter->Render(m_data->context, 
                m_data->cameraPosition, m_data->cameraUp);
        }
    }

    EnableDepthWrite(true);
}

void DirectxEngine::RenderNormalMap(const PostProcessing& post, float timer)
{
    m_data->normalTarget.SetActive(m_data->context);

    for (auto& mesh : m_data->meshes)
    {
        if(UpdateShader(mesh->GetMesh(), post))
        {
            mesh->Render(m_data->context);
        }
    }

    for (auto& mesh : m_data->waters)
    {
        if (UpdateShader(mesh->GetWater(), post, timer))
        {
            mesh->Render(m_data->context);
        }
    }
}

void DirectxEngine::RenderSceneBlur(const PostProcessing& post)
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false);

    auto& blurShader = m_data->shaders[BLUR_SHADER];
    blurShader->SetActive(m_data->context);

    blurShader->UpdateConstantFloat("blurStep", &post.blurStep, 1);
    blurShader->UpdateConstantFloat("blurAmount", &post.blurAmount, 1);
    blurShader->UpdateConstantFloat("weightMain", &post.weights[0], 1);
    blurShader->UpdateConstantFloat("weightOffset", &post.weights[1], 4);

    // Render first horizontal blur pass
    m_data->blurTargetP1.SetActive(m_data->context);
    float horizontalPass = 1.0f;
    float verticalPass = 0.0f;
    blurShader->UpdateConstantFloat("verticalPass", &verticalPass, 1);
    blurShader->UpdateConstantFloat("horizontalPass", &horizontalPass, 1);
    blurShader->SendConstants(m_data->context);

    m_data->sceneTarget.SendTexture(m_data->context, 0);
    m_data->quad.Render(m_data->context);
    m_data->sceneTarget.ClearTexture(m_data->context, 0);

    // Render second vertical blur pass
    m_data->blurTargetP2.SetActive(m_data->context);
    horizontalPass = 0.0f;
    verticalPass = 1.0f;
    blurShader->UpdateConstantFloat("verticalPass", &verticalPass, 1);
    blurShader->UpdateConstantFloat("horizontalPass", &horizontalPass, 1);
    blurShader->SendConstants(m_data->context);
    
    m_data->blurTargetP1.SendTexture(m_data->context, 0);
    m_data->quad.Render(m_data->context);
    m_data->blurTargetP1.ClearTexture(m_data->context, 0);
}

void DirectxEngine::RenderPostProcessing(const PostProcessing& post)
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false);

    auto& postShader = m_data->shaders[POST_SHADER];
    postShader->SetActive(m_data->context);
    m_data->backBuffer.SetActive(m_data->context);

    m_data->sceneTarget.SendTexture(m_data->context, PostProcessing::SCENE);
    m_data->normalTarget.SendTexture(m_data->context, PostProcessing::NORMAL);
    m_data->blurTargetP2.SendTexture(m_data->context, PostProcessing::BLUR);

    postShader->UpdateConstantFloat("fadeAmount", &m_data->fadeAmount, 1);
    postShader->UpdateConstantFloat("glowAmount", &post.glowAmount, 1);
    postShader->UpdateConstantFloat("contrast", &post.contrast, 1);
    postShader->UpdateConstantFloat("saturation", &post.saturation, 1);
    postShader->UpdateConstantFloat("dofDistance", &post.dofDistance, 1);
    postShader->UpdateConstantFloat("dofFade", &post.dofFade, 1);
    postShader->UpdateConstantFloat("fogDistance", &post.fogDistance, 1);
    postShader->UpdateConstantFloat("fogFade", &post.fogFade, 1);
    postShader->UpdateConstantFloat("fogColor", &post.fogColour.r, 3);
    postShader->UpdateConstantFloat("minimumColor", &post.minimumColour.r, 3);
    postShader->UpdateConstantFloat("maximumColor", &post.maximumColour.r, 3);

    postShader->UpdateConstantFloat("finalMask", &post.masks[PostProcessing::FINAL_MAP], 1);
    postShader->UpdateConstantFloat("sceneMask", &post.masks[PostProcessing::SCENE_MAP], 1);
    postShader->UpdateConstantFloat("normalMask", &post.masks[PostProcessing::NORMAL_MAP], 1);
    postShader->UpdateConstantFloat("depthMask", &post.masks[PostProcessing::DEPTH_MAP], 1);
    postShader->UpdateConstantFloat("glowMask", &post.masks[PostProcessing::GLOW_MAP], 1);
    postShader->UpdateConstantFloat("blurGlowMask", &post.masks[PostProcessing::BLUR_GLOW_MAP], 1);
    postShader->UpdateConstantFloat("blurSceneMask", &post.masks[PostProcessing::BLUR_SCENE_MAP], 1);
    postShader->UpdateConstantFloat("depthOfFieldMask", &post.masks[PostProcessing::DOF_MAP], 1);
    postShader->UpdateConstantFloat("fogMask", &post.masks[PostProcessing::FOG_MAP], 1);

    postShader->SendConstants(m_data->context);
    m_data->quad.Render(m_data->context);

    m_data->sceneTarget.ClearTexture(m_data->context, PostProcessing::SCENE);
    m_data->normalTarget.ClearTexture(m_data->context, PostProcessing::NORMAL);
    m_data->blurTargetP2.ClearTexture(m_data->context, PostProcessing::BLUR);
}

void DirectxEngine::UpdateShader(const D3DXMATRIX& world, const Colour& colour)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->UpdateConstantMatrix("world", world);
    shader->UpdateConstantFloat("meshColour", &colour.r, 3);
    shader->SendConstants(m_data->context);
}

bool DirectxEngine::UpdateShader(const Mesh& mesh, 
                                 const PostProcessing& post)
{
    const int index = mesh.normalIndex;
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if (index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
            shader->UpdateConstantFloat("depthNear", &post.depthNear, 1);
            shader->UpdateConstantFloat("depthFar", &post.depthFar, 1);
        }

        shader->UpdateConstantFloat("meshBump", &mesh.bump, 1);
        shader->SendConstants(m_data->context);

        SendTexture(0, mesh.textureIDs[Texture::NORMAL]);
        EnableBackfaceCull(mesh.backfacecull);
        EnableAlphaBlending(false);

        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Mesh& mesh, 
                                 const std::vector<Light>& lights)
{
    const int index = mesh.shaderIndex;
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
            shader->UpdateConstantFloat("cameraPosition", &m_data->cameraPosition.x, 3);
            SendLights(lights);
        }

        shader->UpdateConstantFloat("meshAmbience", &mesh.ambience, 1);
        shader->UpdateConstantFloat("meshBump", &mesh.bump, 1);
        shader->UpdateConstantFloat("meshGlow", &mesh.glow, 1);
        shader->UpdateConstantFloat("meshSpecularity", &mesh.specularity, 1);
        shader->SendConstants(m_data->context);

        SendTextures(mesh.textureIDs);
        EnableBackfaceCull(mesh.backfacecull);
        EnableAlphaBlending(false);

        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Water& water, 
                                 const PostProcessing& post,
                                 float timer)
{
    const int index = water.normalIndex;
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
            shader->UpdateConstantFloat("depthNear", &post.depthNear, 1);
            shader->UpdateConstantFloat("depthFar", &post.depthFar, 1);
            shader->UpdateConstantFloat("timer", &timer, 1);
        }

        shader->UpdateConstantFloat("speed", &water.speed, 1);
        shader->UpdateConstantFloat("bumpIntensity", &water.bump, 1);
        shader->UpdateConstantFloat("bumpVelocity", &water.bumpVelocity.x, 2);
        shader->UpdateConstantFloat("uvScale", &water.uvScale.x, 2);
        SendWaves(water.waves);

        shader->SendConstants(m_data->context);

        EnableBackfaceCull(true);
        EnableAlphaBlending(false);
        SendTexture(0, water.textureIDs[Texture::NORMAL]);

        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Water& water, 
                                 const std::vector<Light>& lights,
                                 float timer)
{
    const int index = water.shaderIndex;
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
            shader->UpdateConstantFloat("timer", &timer, 1);
            shader->UpdateConstantFloat("blendFactor", &m_data->blendFactor, 1);
            shader->UpdateConstantFloat("cameraPosition", &m_data->cameraPosition.x, 3);
            SendLights(lights);
        }

        shader->UpdateConstantFloat("speed", &water.speed, 1);
        shader->UpdateConstantFloat("bumpIntensity", &water.bump, 1);
        shader->UpdateConstantFloat("bumpVelocity", &water.bumpVelocity.x, 2);
        shader->UpdateConstantFloat("uvScale", &water.uvScale.x, 2);
        shader->UpdateConstantFloat("deepColor", &water.deepColour.r, 4);
        shader->UpdateConstantFloat("shallowColor", &water.shallowColour.r, 4);
        shader->UpdateConstantFloat("reflectionTint", &water.reflectionTint.r, 3);
        shader->UpdateConstantFloat("reflectionIntensity", &water.reflection, 1);
        shader->UpdateConstantFloat("fresnal", &water.fresnal.x, 3);
        SendWaves(water.waves);

        shader->SendConstants(m_data->context);

        EnableBackfaceCull(true);
        EnableAlphaBlending(true);
        SendTextures(water.textureIDs);

        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Emitter& emitter)
{
    const int index = emitter.shaderIndex;
    if (index != NO_INDEX)
    {
        auto& shader = m_data->shaders[index];
        if (index != m_data->selectedShader)
        {
            SetSelectedShader(index);
        }

        shader->UpdateConstantFloat("tint", &emitter.tint.r, 4);

        EnableBackfaceCull(false);
        EnableAlphaBlending(true);

        return true;
    }
    return false;
}

void DirectxEngine::UpdateShader(const D3DXMATRIX& world, const Particle& particle)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->UpdateConstantMatrix("worldViewProjection", world * m_data->viewProjection);
    shader->UpdateConstantFloat("alpha", &particle.alpha, 1);
    shader->SendConstants(m_data->context);
    SendTexture(0, particle.texture);
}

void DirectxEngine::SendWaves(const std::vector<Wave>& waves)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0; i < waves.size(); ++i)
    {
        const int offset = i*4; // Arrays pack in buffer of float4
        shader->UpdateConstantFloat("waveFrequency", &waves[i].amplitude, 1, offset);
        shader->UpdateConstantFloat("waveAmplitude", &waves[i].amplitude, 1, offset);
        shader->UpdateConstantFloat("wavePhase", &waves[i].phase, 1, offset);
        shader->UpdateConstantFloat("waveDirectionX", &waves[i].directionX, 1, offset);
        shader->UpdateConstantFloat("waveDirectionZ", &waves[i].directionZ, 1, offset);
    }
}

void DirectxEngine::SendLights(const std::vector<Light>& lights)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i*4; // Arrays pack in buffer of float4
        shader->UpdateConstantFloat("lightSpecularity", &lights[i].specularity, 1, offset);
        shader->UpdateConstantFloat("lightAttenuation", &lights[i].attenuation.x, 3, offset);
        shader->UpdateConstantFloat("lightPosition", &lights[i].position.x, 3, offset);
        shader->UpdateConstantFloat("lightDiffuse", &lights[i].diffuse.r, 3, offset);
        shader->UpdateConstantFloat("lightSpecular", &lights[i].specular.r, 3, offset);
    }
}

void DirectxEngine::SendTextures(const std::vector<int>& textures)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0, slot = 0; i < textures.size(); ++i)
    {
        const Texture::Type type = static_cast<Texture::Type>(i);
        if (SendTexture(slot, textures[type]))
        {
            ++slot;
        }
    }
}

bool DirectxEngine::SendTexture(int slot, int ID)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    if(ID != NO_INDEX && shader->HasTextureSlot(slot))
    {
        m_data->textures[ID]->SendTexture(m_data->context, slot);
        return true;
    }
    return false;
}

void DirectxEngine::SetSelectedShader(int index)
{
    m_data->selectedShader = index;
    m_data->shaders[index]->SetActive(m_data->context);
}

ID3D11Device* DirectxEngine::GetDevice() const
{
    return m_data->device;
}

std::string DirectxEngine::GetName() const
{
    return "DirectX";
}

void DirectxEngine::UpdateView(const Matrix& world)
{
    D3DXMatrixIdentity(&m_data->view);

    m_data->view._11 = world.m11;
    m_data->view._21 = world.m12;
    m_data->view._31 = -world.m13;

    m_data->view._12 = world.m21;
    m_data->view._22 = world.m22;
    m_data->view._32 = -world.m23;

    m_data->view._13 = world.m31;
    m_data->view._23 = world.m32;
    m_data->view._33 = -world.m33;

    m_data->view._41 = world.m14;
    m_data->view._42 = world.m24;
    m_data->view._43 = world.m34;

    m_data->cameraPosition.x = m_data->view._41;
    m_data->cameraPosition.y = m_data->view._42;
    m_data->cameraPosition.z = m_data->view._43;

    m_data->cameraUp.x = m_data->view._21;
    m_data->cameraUp.y = m_data->view._22;
    m_data->cameraUp.z = m_data->view._23;

    D3DXMatrixInverse(&m_data->view, nullptr, &m_data->view);
    m_data->viewProjection = m_data->view * m_data->projection;
}

std::string DirectxEngine::GetShaderText(int index) const
{
    return m_data->shaders[index]->GetText();
}

std::string DirectxEngine::GetShaderAssembly(int index)
{
    return m_data->shaders[index]->GetAssembly();
}

void DirectxEngine::SetFade(float value)
{
    m_data->fadeAmount = value;
}

void DirectxEngine::WriteToShader(const std::string& name,
                                  const std::string& text)
{
    const std::string filepath = GENERATED_PATH + name + HLSL_SHADER_EXTENSION;
    std::ofstream file(filepath.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!file.is_open())
    {
        Logger::LogError("Could not open " + filepath);
    }
    else
    {
        file << text << std::endl;
        file.close();
    }
}

void DirectxEngine::EnableDepthWrite(bool enable)
{
    if (enable != m_data->isDepthWrite)
    {
        m_data->isDepthWrite = enable;
        m_data->context->OMSetDepthStencilState(
            enable ? m_data->writeState : m_data->noWriteState, 0xFFFFFFFF);
    }
}

void DirectxEngine::EnableAlphaBlending(bool enable)
{
    if (enable != m_data->isAlphaBlend)
    {
        m_data->isAlphaBlend = enable;
	    m_data->context->OMSetBlendState(
            enable ? m_data->alphaBlendState : m_data->noBlendState, 0, 0xFFFFFFFF);
    }
}

void DirectxEngine::EnableBackfaceCull(bool enable)
{
    if(enable != m_data->isBackfaceCull)
    {
        m_data->isBackfaceCull = enable;
        m_data->context->RSSetState(
            enable ? m_data->cullState : m_data->noCullState);
    }
}