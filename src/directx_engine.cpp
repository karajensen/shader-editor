////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_engine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directx_engine.h"
#include "directx_common.h"
#include "directx_shader.h"
#include "directx_mesh.h"
#include "directx_texture.h"
#include "directx_emitter.h"
#include "directx_target.h"
#include "scene_interface.h"
#include "logger.h"

#include <array>
#include <fstream>

/**
* Draw states available for rendering
*/
namespace Rasterizer
{
    enum State
    {
        None,
        BackfaceCull,
        BackfaceCullWire,
        NoCull,
        NoCullWire,
        Max
    };
}

/**
* Samplers states available for textures
*/
namespace Sampler
{
    enum State
    {
        Nearest,
        Linear,
        Anisotropic,
        Max
    };
}

/**
* Internal data for the directx rendering engine
*/
struct DirectxData
{
    DirectxData();
    ~DirectxData();

    /**
    * Releases the device/context
    */
    void Release();

    ID3D11BlendState* alphaBlendState = nullptr;     ///< State for alpha blending
    ID3D11BlendState* alphaBlendMultiply = nullptr;  ///< State for alpha blending with colour multiply
    ID3D11BlendState* noBlendState = nullptr;        ///< State for no alpha blending
    ID3D11DepthStencilState* writeState = nullptr;   ///< State for writing to the depth buffer
    ID3D11DepthStencilState* noWriteState = nullptr; ///< State for not writing to the depth buffer
    IDXGISwapChain* swapchain = nullptr;             ///< Collection of buffers for displaying frames
    ID3D11Device* device = nullptr;                  ///< Direct3D device interface
    ID3D11DeviceContext* context = nullptr;          ///< Direct3D device context
    ID3D11Debug* debug = nullptr;                    ///< Direct3D debug interface, only created in debug
    std::vector<ID3D11RasterizerState*> drawStates;  ///< Rasterizer states
    std::vector<ID3D11SamplerState*> samplers;       ///< Texture sampler states
    Rasterizer::State drawState;                     ///< The current state of the rasterizer

    DxQuad quad;                         ///< Quad to render the final post processed scene onto
    DxRenderTarget backBuffer;           ///< Render target for the back buffer
    DxRenderTarget sceneTarget;          ///< Render target for the main scene
    DxRenderTarget preEffectsTarget;     ///< Render target for pre-rendering effects
    DxRenderTarget blurTarget;           ///< Render target for blurring the main scene
    D3DXMATRIX view;                     ///< View matrix
    D3DXMATRIX projection;               ///< Projection matrix
    D3DXMATRIX viewProjection;           ///< View projection matrix
    D3DXVECTOR3 cameraPosition;          ///< Position of the camera
    D3DXVECTOR3 cameraUp;                ///< Up vector of the camera
    bool isBackfaceCull = false;         ///< Whether the culling rasterize state is active
    bool isAlphaBlend = false;           ///< Whether alpha blending is currently active
    bool isBlendMultiply = false;        ///< Whether to multiply the blend colours
    bool isDepthWrite = false;           ///< Whether writing to the depth buffer is active
    bool isWireframe = false;            ///< Whether to render the scene as wireframe
    bool useDiffuseTextures = true;      ///< Whether to render diffuse textures
    int selectedShader = -1;             ///< currently selected shader for rendering the scene
    float fadeAmount = 0.0f;             ///< the amount to fade the scene by
    
    std::unique_ptr<DxQuadMesh> shadows;              ///< Shadow instances
    std::vector<std::unique_ptr<DxTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<DxMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<DxMesh>> waters;      ///< Each water in the scene
    std::vector<std::unique_ptr<DxMesh>> terrain;     ///< Each terrain in the scene
    std::vector<std::unique_ptr<DxShader>> shaders;   ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<DxEmitter>> emitters; ///< Particle emitters
};

DirectxData::DirectxData()
    : sceneTarget("SceneTarget", SCENE_TEXTURES, true)
    , blurTarget("BlurTarget", BLUR_TEXTURES, false, true)
    , preEffectsTarget("PreEffectsTarget", EFFECTS_TEXTURES, false)
    , backBuffer("BackBuffer")
    , quad("SceneQuad")
    , drawState(Rasterizer::None)
{
    samplers.resize(Sampler::Max);
    samplers.assign(Sampler::Max, nullptr);

    drawStates.resize(Rasterizer::Max);
    drawStates.assign(Rasterizer::Max, nullptr);
}

DirectxData::~DirectxData()
{
    Release();
}

void DirectxData::Release()
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

    for (auto& emitter : emitters)
    {
        emitter->Release();
    }

    quad.Release();
    sceneTarget.Release();
    blurTarget.Release();
    backBuffer.Release();
    preEffectsTarget.Release();

    for (unsigned int i = 0; i < drawStates.size(); ++i)
    {
        SafeRelease(&drawStates[i]);
    }

    for (unsigned int i = 0; i < samplers.size(); ++i)
    {
        SafeRelease(&samplers[i]);
    }

    SafeRelease(&noBlendState);
    SafeRelease(&alphaBlendState);
    SafeRelease(&alphaBlendMultiply);
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

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &scd,
        &m_data->swapchain, &m_data->device, nullptr, &m_data->context)))
    {
        Logger::LogError("DirectX: Device creation failed");
        return false;
    }

    InitialiseDebugging();

    // Create the post processing quad
    m_data->quad.Initialise(m_data->device, m_data->context);

    // Initialise all states
    if (!InitialiseBlendStates() ||
        !InitialiseDrawStates() ||
        !InitialiseSamplerStates() ||
        !InitialiseDepthStates())
    {
        Logger::LogError("DirectX: Failed to initialise states");
        return false;
    }

    // Create the render targets. Back buffer must be initialised first.
    m_data->sceneTarget.SetHighQuality(DEPTH_ID); // Required for DOF
    if (!m_data->backBuffer.Initialise(m_data->device, m_data->swapchain) ||
        !m_data->sceneTarget.Initialise(m_data->device, m_data->samplers[Sampler::Linear]) ||
        !m_data->preEffectsTarget.Initialise(m_data->device, m_data->samplers[Sampler::Linear]) ||
        !m_data->blurTarget.Initialise(m_data->device, m_data->samplers[Sampler::Linear]))
    {
        Logger::LogError("DirectX: Failed to create render targets");
        return false;
    }

    // Setup the directX environment
    m_data->drawState = Rasterizer::None;
    m_data->isAlphaBlend = true;
    m_data->isBlendMultiply = true;
    m_data->isDepthWrite = false;
    m_data->isWireframe = false;
    SetRenderState(true, false);
    EnableAlphaBlending(false, false);
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
        RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);

    SetDebugName(m_data->device, "Device");
    SetDebugName(m_data->context, "Context");
    SetDebugName(m_data->swapchain, "SwapChain");

    Logger::LogInfo("DirectX: D3D11 sucessful");
    return true;
}

bool DirectxEngine::InitialiseBlendStates()
{
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;

    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        blendDesc.RenderTarget[i].BlendEnable = FALSE;
        blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE; 
        blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    if (FAILED(m_data->device->CreateBlendState(&blendDesc, &m_data->noBlendState)))
    {
        Logger::LogError("DirectX: Failed to create no blending state");
        return false;
    }

    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        blendDesc.RenderTarget[i].BlendEnable = TRUE;
        blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE; 
        blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO; 
    }

    if (FAILED(m_data->device->CreateBlendState(&blendDesc, &m_data->alphaBlendState)))
    {
        Logger::LogError("DirectX: Failed to create alpha blending state");
        return false;
    }

    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_DEST_COLOR;
        blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA; 
        blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO; 
    }

    if (FAILED(m_data->device->CreateBlendState(&blendDesc, &m_data->alphaBlendMultiply)))
    {
        Logger::LogError("DirectX: Failed to create alpha blending state");
        return false;
    }

    SetDebugName(m_data->noBlendState, "NoBlendState");
    SetDebugName(m_data->alphaBlendState, "AlphaBlendState");
    SetDebugName(m_data->alphaBlendMultiply, "AlphaBlendMultiplyState");
    return true;
}

bool DirectxEngine::InitialiseDepthStates()
{
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

    SetDebugName(m_data->writeState, "DepthWriteState");
    SetDebugName(m_data->noWriteState, "NoDepthWriteState");
    return true;
}

bool DirectxEngine::InitialiseSamplerStates()
{
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = MAX_ANISOTROPY;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if(FAILED(m_data->device->CreateSamplerState(
        &samplerDesc, &m_data->samplers[Sampler::Anisotropic])))
    {
        Logger::LogError("Failed to create anisotropic texture sampler");
        return false;
    }

    samplerDesc.MaxAnisotropy = 0;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    if(FAILED(m_data->device->CreateSamplerState(
        &samplerDesc, &m_data->samplers[Sampler::Linear])))
    {
        Logger::LogError("Failed to create linear texture sampler");
        return false;
    }

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    if(FAILED(m_data->device->CreateSamplerState(
        &samplerDesc, &m_data->samplers[Sampler::Nearest])))
    {
        Logger::LogError("Failed to create nearest texture sampler");
        return false;
    }

    return true;
}

bool DirectxEngine::InitialiseDrawStates()
{
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

    if(FAILED(m_data->device->CreateRasterizerState(
        &rasterDesc, &m_data->drawStates[Rasterizer::BackfaceCull])))
    {
        Logger::LogError("DirectX: Failed to create cull rasterizer state");
        return false;
    }

    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    if(FAILED(m_data->device->CreateRasterizerState(
        &rasterDesc, &m_data->drawStates[Rasterizer::BackfaceCullWire])))
    {
        Logger::LogError("DirectX: Failed to create cull wire rasterizer state");
        return false;
    }

    rasterDesc.CullMode = D3D11_CULL_NONE;
    if (FAILED(m_data->device->CreateRasterizerState(
        &rasterDesc, &m_data->drawStates[Rasterizer::NoCullWire])))
    {
        Logger::LogError("DirectX: Failed to create no cull wire rasterizer state");
        return false;
    }

    rasterDesc.FillMode = D3D11_FILL_SOLID;
    if (FAILED(m_data->device->CreateRasterizerState(
        &rasterDesc, &m_data->drawStates[Rasterizer::NoCull])))
    {
        Logger::LogError("DirectX: Failed to create no cull rasterizer state");
        return false;
    }

    SetDebugName(m_data->drawStates[Rasterizer::BackfaceCull], "BackfaceCull");
    SetDebugName(m_data->drawStates[Rasterizer::BackfaceCullWire], "BackfaceCullWire");
    SetDebugName(m_data->drawStates[Rasterizer::NoCull], "NoCull");
    SetDebugName(m_data->drawStates[Rasterizer::NoCullWire], "NoCullWire");
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
    m_data->shadows = std::make_unique<DxQuadMesh>(scene.Shadows(),
        [this](const D3DXMATRIX& world, int texture){ UpdateShader(world, texture); });

    m_data->textures.reserve(scene.Textures().size());
    for(const auto& texture : scene.Textures())
    {
        m_data->textures.push_back(std::unique_ptr<DxTexture>(
            new DxTexture(*texture)));
    }

    m_data->shaders.reserve(scene.Shaders().size());
    for(const auto& shader : scene.Shaders())
    {
        m_data->shaders.push_back(std::unique_ptr<DxShader>(
            new DxShader(*shader)));
    }

    m_data->meshes.reserve(scene.Meshes().size());
    for(const auto& mesh : scene.Meshes())
    {
        m_data->meshes.push_back(std::unique_ptr<DxMesh>(new DxMesh(*mesh,
            [this](const D3DXMATRIX& world, int texture){ UpdateShader(world, texture); })));
    }

    m_data->terrain.reserve(scene.Terrains().size());
    for(const auto& terrain : scene.Terrains())
    {
        m_data->terrain.push_back(std::unique_ptr<DxMesh>(new DxMesh(*terrain,
            [this](const D3DXMATRIX& world, int texture){ UpdateShader(world, texture); })));
    }

    m_data->waters.reserve(scene.Waters().size());
    for(const auto& water : scene.Waters())
    {
        m_data->waters.push_back(std::unique_ptr<DxMesh>(new DxMesh(*water,
            [this](const D3DXMATRIX& world, int texture){ UpdateShader(world, texture); })));
    }

    m_data->emitters.reserve(scene.Emitters().size());
    for(const auto& emitter : scene.Emitters())
    {
        m_data->emitters.push_back(std::unique_ptr<DxEmitter>(new DxEmitter(*emitter,
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

    for(auto& terrain : m_data->terrain)
    {
        terrain->Initialise(m_data->device, m_data->context);
    }

    for(auto& texture : m_data->textures)
    {
        texture->Initialise(m_data->device);
    }

    for(auto& emitter : m_data->emitters)
    {
        emitter->Initialise(m_data->device, m_data->context);
    }

    m_data->shadows->Initialise(m_data->device, m_data->context);
    
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
    RenderSceneMap(scene, timer);
    RenderPreEffects(scene.Post());
    RenderBlur(scene.Post());
    RenderPostProcessing(scene.Post());
    m_data->swapchain->Present(0, 0);
}

void DirectxEngine::RenderSceneMap(const IScene& scene, float timer)
{
    m_data->sceneTarget.SetActive(m_data->context);

    RenderTerrain(scene);
    RenderShadows();
    RenderMeshes(scene);
    RenderWater(scene, timer);
    RenderEmitters(scene);
}

void DirectxEngine::RenderMeshes(const IScene& scene)
{
    for (auto& mesh : m_data->meshes)
    {
        if (UpdateShader(mesh->GetMesh(), scene))
        {
            mesh->Render(m_data->context);
        }
    }
}

void DirectxEngine::RenderTerrain(const IScene& scene)
{
    for (auto& terrain : m_data->terrain)
    {
        if (UpdateShader(terrain->GetTerrain(), scene))
        {
            terrain->Render(m_data->context);
        }
    }
}

void DirectxEngine::RenderWater(const IScene& scene, float timer)
{
    for (auto& water : m_data->waters)
    {
        if (UpdateShader(water->GetWater(), scene, timer))
        {
            water->Render(m_data->context);
        }
    }
}

void DirectxEngine::RenderShadows()
{
    if (UpdateShader(m_data->shadows->GetData()))
    {
        EnableDepthWrite(false);

        m_data->shadows->Render(m_data->context);

        EnableDepthWrite(true);
    }
}

void DirectxEngine::RenderEmitters(const IScene& scene)
{
    EnableDepthWrite(false);

    for (auto& emitter : m_data->emitters)
    {
        if (UpdateShader(emitter->GetEmitter(), scene))
        {
            emitter->Render(m_data->context, 
                m_data->cameraPosition, m_data->cameraUp);
        }
    }

    EnableDepthWrite(true);
}

void DirectxEngine::RenderPreEffects(const PostProcessing& post)
{
    SetRenderState(false, false);
    EnableAlphaBlending(false, false);

    m_data->preEffectsTarget.SetActive(m_data->context);

    SetSelectedShader(ShaderIndex::Pre);
    auto& preShader = m_data->shaders[ShaderIndex::Pre];
    
    preShader->UpdateConstantFloat("bloomStart", &post.BloomStart(), 1);
    preShader->UpdateConstantFloat("bloomFade", &post.BloomFade(), 1);
    preShader->SendConstants(m_data->context);

    preShader->SendTexture(m_data->context, 0, m_data->sceneTarget, SCENE_ID);
    
    m_data->quad.Render(m_data->context);

    preShader->ClearTexture(m_data->context, 0);
}

void DirectxEngine::RenderBlur(const PostProcessing& post)
{
    SetRenderState(false, false);
    EnableAlphaBlending(false, false);

    m_data->blurTarget.SetActive(m_data->context);

    SetSelectedShader(ShaderIndex::BlurHorizontal);
    auto& blurHorizontal = m_data->shaders[ShaderIndex::BlurHorizontal];

    blurHorizontal->UpdateConstantFloat("blurStep", &post.BlurStep(), 1);
    blurHorizontal->SendConstants(m_data->context);

    blurHorizontal->SendTexture(m_data->context, 0, m_data->preEffectsTarget);

    m_data->quad.Render(m_data->context);

    blurHorizontal->ClearTexture(m_data->context, 0);

    SetSelectedShader(ShaderIndex::BlurVertical);
    auto& blurVertical = m_data->shaders[ShaderIndex::BlurVertical];

    blurVertical->UpdateConstantFloat("blurStep", &post.BlurStep(), 1);
    blurVertical->SendConstants(m_data->context);

    m_data->blurTarget.CopyTextures(m_data->context);
    
    blurVertical->SendCopiedTexture(m_data->context, 0, m_data->blurTarget);
    
    m_data->quad.Render(m_data->context);
    
    blurHorizontal->ClearTexture(m_data->context, 0);
}

void DirectxEngine::RenderPostProcessing(const PostProcessing& post)
{
    m_data->useDiffuseTextures = post.UseDiffuseTextures();

    SetRenderState(false, false);
    EnableAlphaBlending(false, false);

    SetSelectedShader(ShaderIndex::Post);
    auto& postShader = m_data->shaders[ShaderIndex::Post];

    m_data->backBuffer.SetActive(m_data->context);

    postShader->SendTexture(m_data->context, 0, m_data->preEffectsTarget, SCENE_ID);
    postShader->SendTexture(m_data->context, 1, m_data->blurTarget, BLUR_ID);
    postShader->SendTexture(m_data->context, 2, m_data->sceneTarget, DEPTH_ID);

    postShader->UpdateConstantFloat("bloomIntensity", &post.BloomIntensity(), 1);
    postShader->UpdateConstantFloat("fadeAmount", &m_data->fadeAmount, 1);
    postShader->UpdateConstantFloat("contrast", &post.Contrast(), 1);
    postShader->UpdateConstantFloat("saturation", &post.Saturation(), 1);
    postShader->UpdateConstantFloat("dofStart", &post.DOFStart(), 1);
    postShader->UpdateConstantFloat("dofFade", &post.DOFFade(), 1);
    postShader->UpdateConstantFloat("fogStart", &post.FogStart(), 1);
    postShader->UpdateConstantFloat("fogFade", &post.FogFade(), 1);
    postShader->UpdateConstantFloat("fogColor", &post.FogColour().r, 3);
    postShader->UpdateConstantFloat("minimumColor", &post.MinColour().r, 3);
    postShader->UpdateConstantFloat("maximumColor", &post.MaxColour().r, 3);

    postShader->UpdateConstantFloat("finalMask", &post.Mask(PostProcessing::Final), 1);
    postShader->UpdateConstantFloat("sceneMask", &post.Mask(PostProcessing::Scene), 1);
    postShader->UpdateConstantFloat("depthMask", &post.Mask(PostProcessing::Depth), 1);
    postShader->UpdateConstantFloat("blurSceneMask", &post.Mask(PostProcessing::Blur), 1);
    postShader->UpdateConstantFloat("depthOfFieldMask", &post.Mask(PostProcessing::Dof), 1);
    postShader->UpdateConstantFloat("fogMask", &post.Mask(PostProcessing::Fog), 1);
    postShader->UpdateConstantFloat("bloomMask", &post.Mask(PostProcessing::Bloom), 1);

    postShader->SendConstants(m_data->context);
    m_data->quad.Render(m_data->context);

    postShader->ClearTexture(m_data->context, 0);
    postShader->ClearTexture(m_data->context, 1);
    postShader->ClearTexture(m_data->context, 2);
}

void DirectxEngine::UpdateShader(const D3DXMATRIX& world, int texture)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->UpdateConstantMatrix("world", world);
    shader->SendConstants(m_data->context);
    SendTexture(0, m_data->useDiffuseTextures ? texture : TextureIndex::BlankTexture);
}

bool DirectxEngine::UpdateShader(const MeshData& quad)
{
    const int index = quad.ShaderID();
    if (index != -1)
    {
        auto& shader = m_data->shaders[index];
        if(index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
        }

        SetRenderState(false, m_data->isWireframe);
        EnableAlphaBlending(true, true);
        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const MeshData& mesh, 
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
            shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
            shader->UpdateConstantFloat("cameraPosition", &m_data->cameraPosition.x, 3);
            shader->UpdateConstantFloat("depthNear", &scene.Post().DepthNear(), 1);
            shader->UpdateConstantFloat("depthFar", &scene.Post().DepthFar(), 1);

            if (index == ShaderIndex::Water)
            {
                shader->UpdateConstantFloat("timer", &timer, 1);
            }
        }

        SendTextures(mesh.TextureIDs());
        SetRenderState(mesh.BackfaceCull(), m_data->isWireframe);
        EnableAlphaBlending(alphaBlend, false);
        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Terrain& terrain, 
                                 const IScene& scene)
{
    if (UpdateShader(terrain, scene, false))
    {
        SendAttributes(terrain);
        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Mesh& mesh, const IScene& scene)
{
    if (UpdateShader(mesh, scene, false))
    {
        SendAttributes(mesh);
        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Water& water, 
                                 const IScene& scene, 
                                 float timer)
{
    if (UpdateShader(water, scene, true, timer))
    {
        auto& shader = m_data->shaders[water.ShaderID()];
        shader->UpdateConstantFloat("speed", &water.Speed(), 1);
        shader->UpdateConstantFloat("bumpIntensity", &water.Bump(), 1);
        shader->UpdateConstantFloat("bumpScale", &water.BumpScale().x, 2);
        shader->UpdateConstantFloat("uvScale", &water.UVScale().x, 2);
        shader->UpdateConstantFloat("deepColor", &water.Deep().r, 4);
        shader->UpdateConstantFloat("shallowColor", &water.Shallow().r, 4);
        shader->UpdateConstantFloat("reflectionTint", &water.ReflectionTint().r, 3);
        shader->UpdateConstantFloat("reflectionIntensity", &water.ReflectionIntensity(), 1);
        shader->UpdateConstantFloat("fresnal", &water.Fresnal().x, 3);

        const auto& waves = water.Waves();
        for (unsigned int i = 0; i < waves.size(); ++i)
        {
            const int offset = i*4; // Arrays pack in buffer of float4
            shader->UpdateConstantFloat("waveFrequency", &waves[i].amplitude, 1, offset);
            shader->UpdateConstantFloat("waveAmplitude", &waves[i].frequency, 1, offset);
            shader->UpdateConstantFloat("wavePhase", &waves[i].phase, 1, offset);
            shader->UpdateConstantFloat("waveDirectionX", &waves[i].directionX, 1, offset);
            shader->UpdateConstantFloat("waveDirectionZ", &waves[i].directionZ, 1, offset);
        }
        return true;
    }
    return false;
}

bool DirectxEngine::UpdateShader(const Emitter& emitter, const IScene& scene)
{
    const int index = emitter.ShaderID();
    if (index != -1)
    {
        auto& shader = m_data->shaders[index];
        if (index != m_data->selectedShader)
        {
            SetSelectedShader(index);
            shader->UpdateConstantFloat("depthNear", &scene.Post().DepthNear(), 1);
            shader->UpdateConstantFloat("depthFar", &scene.Post().DepthFar(), 1);
        }

        shader->UpdateConstantFloat("tint", &emitter.Tint().r, 4);

        SetRenderState(false, m_data->isWireframe);
        EnableAlphaBlending(true, false);

        return true;
    }
    return false;
}

void DirectxEngine::UpdateShader(const D3DXMATRIX& world, const Particle& particle)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->UpdateConstantMatrix("worldViewProjection", world * m_data->viewProjection);
    shader->UpdateConstantFloat("alpha", &particle.Alpha(), 1);
    shader->SendConstants(m_data->context);
    SendTexture(0, particle.Texture());
}

void DirectxEngine::SendAttributes(const MeshAttributes& attributes)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    shader->UpdateConstantFloat("meshCausticAmount", &attributes.CausticsAmount(), 1);
    shader->UpdateConstantFloat("meshCausticScale", &attributes.CausticsScale(), 1);
    shader->UpdateConstantFloat("meshAmbience", &attributes.Ambience(), 1);
    shader->UpdateConstantFloat("meshBump", &attributes.Bump(), 1);
    shader->UpdateConstantFloat("meshSpecularity", &attributes.Specularity(), 1);
    shader->UpdateConstantFloat("meshSpecular", &attributes.Specular(), 1);
    shader->UpdateConstantFloat("meshDiffuse", &attributes.Diffuse(), 1);
}

void DirectxEngine::SendLights(const std::vector<std::unique_ptr<Light>>& lights)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i*4; // Arrays pack in buffer of float4
        shader->UpdateConstantFloat("lightSpecularity", &lights[i]->Specularity(), 1, offset);
        shader->UpdateConstantFloat("lightAttenuation", &lights[i]->Attenuation().x, 3, offset);
        shader->UpdateConstantFloat("lightPosition", &lights[i]->Position().x, 3, offset);
        shader->UpdateConstantFloat("lightDiffuse", &lights[i]->Diffuse().r, 3, offset);
        shader->UpdateConstantFloat("lightSpecular", &lights[i]->Specular().r, 3, offset);
        shader->UpdateConstantFloat("lightActive", &lights[i]->Active(), 1, offset);
    }
}

void DirectxEngine::SendTextures(const std::vector<int>& textures)
{
    int slot = 1;
    auto& shader = m_data->shaders[m_data->selectedShader];
    slot += SendTexture(slot, textures[TextureSlot::Normal]) ? 1 : 0;
    slot += SendTexture(slot, textures[TextureSlot::Specular]) ? 1 : 0;
    slot += SendTexture(slot, textures[TextureSlot::Environment]) ? 1 : 0;
    slot += SendTexture(slot, textures[TextureSlot::Caustics]) ? 1 : 0;
}

bool DirectxEngine::SendTexture(int slot, int ID)
{
    auto& shader = m_data->shaders[m_data->selectedShader];
    if(ID != -1 && shader->HasTextureSlot(slot))
    {
        auto& texture = m_data->textures[ID];

        Sampler::State state = Sampler::Nearest;
        switch (texture->Filtering())
        {
        case Texture::Linear:
            state = Sampler::Linear;
            break;
        case Texture::Anisotropic:
            state = Sampler::Anisotropic;
            break;
        }

        shader->SendTexture(m_data->context, slot, 
            texture->Get(), &m_data->samplers[state]);

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

void DirectxEngine::WriteToShader(const Shader& shader,
                                  const std::string& text)
{
    std::ofstream file(shader.HLSLShaderFile().c_str(), 
        std::ios_base::out | std::ios_base::trunc);

    if (!file.is_open())
    {
        Logger::LogError("Could not open " + shader.HLSLShaderFile());
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

void DirectxEngine::EnableAlphaBlending(bool enable, bool multiply)
{
    if (enable != m_data->isAlphaBlend || multiply != m_data->isBlendMultiply)
    {
        m_data->isAlphaBlend = enable;
        m_data->isBlendMultiply = multiply;

        m_data->context->OMSetBlendState(enable ? 
            (multiply ? m_data->alphaBlendMultiply : m_data->alphaBlendState)
            : m_data->noBlendState, 0, 0xFFFFFFFF);
    }
}

void DirectxEngine::SetRenderState(bool cull, bool wireframe)
{
    const Rasterizer::State state = cull ?
        (wireframe ? Rasterizer::BackfaceCullWire : Rasterizer::BackfaceCull) :
        (wireframe ? Rasterizer::NoCullWire : Rasterizer::NoCull);

    if (m_data->drawState != state)
    {
        m_data->drawState = state;
        m_data->context->RSSetState(m_data->drawStates[state]);
    }
}

void DirectxEngine::ToggleWireframe()
{
    m_data->isWireframe = !m_data->isWireframe;
}

void DirectxEngine::ReloadTerrain(int index)
{
    const auto& name = m_data->terrain[index]->GetTerrain().Name();
    if (!m_data->terrain[index]->Reload(m_data->context))
    {
        Logger::LogError("Terrain: " + name + " reload failed");
    }
}

void DirectxEngine::ReloadTexture(int index)
{
    const auto& name = m_data->textures[index]->Name();
    m_data->textures[index]->ReloadPixels(m_data->device) ?
        Logger::LogInfo("Texture: " + name + " reload successful") :
        Logger::LogError("Texture: " + name + " reload failed");
}
