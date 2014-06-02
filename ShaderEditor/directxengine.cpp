////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxcommon.h"
#include "directxshader.h"
#include "directxmesh.h"
#include "directxtexture.h"
#include "directxtarget.h"

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

    ID3D11DepthStencilView* zbuffer;      ///< Depth buffer
    ID3D11RasterizerState* cullState;     ///< Normal state of the rasterizer
    ID3D11RasterizerState* nocullState;   ///< No face culling state of the rasterizer
    IDXGISwapChain* swapchain;            ///< Collection of buffers for displaying frames
    ID3D11Device* device;                 ///< Direct3D device interface
    ID3D11DeviceContext* context;         ///< Direct3D device context
    ID3D11Debug* debug;                   ///< Direct3D debug interface, only created in debug

    DxShader postShader;                  ///< Post processing shader
    DxMesh quad;                          ///< Quad to render the final post processed scene onto
    DxRenderTarget backBuffer;            ///< Render target for the back buffer
    DxRenderTarget sceneTarget;           ///< Render target for the main scene

    std::vector<DxTexture> textures;      ///< Textures shared by all meshes
    std::vector<DxMesh> meshes;           ///< Each mesh in the scene
    std::vector<DxShader> shaders;        ///< Shaders shared by all meshes
    D3DXMATRIX view;                      ///< View matrix
    D3DXMATRIX projection;                ///< Projection matrix
    D3DXVECTOR3 camera;                   ///< Position of the camera
    bool isBackfaceCull;                  ///< Whether the culling rasterize state is active
    int selectedShader;                   ///< currently selected shader for rendering the scene
    bool viewUpdated;                     ///< Whether the view matrix has updated this tick
    bool lightsUpdated;                   ///< Whether the lights have been updated this tick
};

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    context(nullptr),
    debug(nullptr),
    zbuffer(nullptr),
    cullState(nullptr),
    nocullState(nullptr),
    isBackfaceCull(true),
    selectedShader(NO_INDEX),
    viewUpdated(true),
    lightsUpdated(true),
    sceneTarget("Scene"),
    backBuffer("BackBuffer", true),
    postShader(POST_FX_PATH, POST_ASM_PATH),
    quad("SceneQuad")
{
}

DirectxData::~DirectxData()
{
    Release();
}

void DirectxData::Release()
{
    viewUpdated = true;
    lightsUpdated = true;
    selectedShader = NO_INDEX;
    isBackfaceCull = true;

    for(DxTexture& texture : textures)
    {
        texture.Release();
    }

    for(DxMesh& mesh : meshes)
    {
        mesh.Release();
    }

    for(DxShader& shader : shaders)
    {
        shader.Release();
    }

    postShader.Release();
    quad.Release();
    sceneTarget.Release();
    backBuffer.Release();

    SafeRelease(&cullState);
    SafeRelease(&nocullState);
    SafeRelease(&zbuffer);
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
}

bool DirectxEngine::Initialize()
{
    m_data->Release();

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  
    scd.OutputWindow = m_hwnd;                            
    scd.SampleDesc.Count = 1;                           
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

    // Create the depth texture
    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = WINDOW_WIDTH;
    depthTextureDesc.Height = WINDOW_HEIGHT;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthTexture;
    if(FAILED(m_data->device->CreateTexture2D(&depthTextureDesc, 0, &depthTexture)))
    {
        Logger::LogError("DirectX: Depth buffer texture creation failed");
        return false;
    }

    // Create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Format = depthTextureDesc.Format;
    depthBufferDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    if(FAILED(m_data->device->CreateDepthStencilView(
        depthTexture, &depthBufferDesc, &m_data->zbuffer)))
    {
        Logger::LogError("DirectX: Depth buffer creation failed");
        return false;
    }
    SetDebugName(depthTexture, "DepthBufferTexture");
    depthTexture->Release();

    // Create the render targets, back buffer needs to be created first
    if(!m_data->backBuffer.Initialise(m_data->device, m_data->swapchain) ||
       !m_data->sceneTarget.Initialise(m_data->device))
    {
        Logger::LogError("DirectX: Failed to create render targets");
        return false;
    }

    // Create the post processing quad and shader
    m_data->quad.Initialise(m_data->device, m_data->context);
    const std::string errors = m_data->postShader.CompileShader(m_data->device);
    if(!errors.empty())
    {
        Logger::LogError("DirectX: Post shader failed: " + errors);
        return false;
    }
    assert(m_data->postShader.HasTextureSlot(0));

    // Setup the directX environment
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
    SetBackfaceCull(true);

    rasterDesc.CullMode = D3D11_CULL_NONE;
    if(FAILED(m_data->device->CreateRasterizerState(&rasterDesc, &m_data->nocullState)))
    {
        Logger::LogError("DirectX: Failed to create no cull rasterizer state");
        return false;
    }

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
        CAMERA_NEAR, CAMERA_FAR);

    SetDebugName(m_data->cullState, "CullState");
    SetDebugName(m_data->nocullState, "NoCullState");
    SetDebugName(m_data->device, "Device");
    SetDebugName(m_data->context, "Context");
    SetDebugName(m_data->swapchain, "SwapChain");
    SetDebugName(m_data->zbuffer, "DepthBuffer");

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
    return m_data->shaders[index].CompileShader(m_data->device);
}

bool DirectxEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                    const std::vector<Mesh>& alpha, 
                                    const std::vector<Shader>& shaders,
                                    const std::vector<Texture>& textures)
{
    m_data->textures.reserve(textures.size());
    for(const Texture& texture : textures)
    {
        m_data->textures.push_back(DxTexture(texture.path));
    }

    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        m_data->shaders.push_back(DxShader(
            shader.index, shader.hlslShaderFile));
    }

    m_data->meshes.reserve(meshes.size());
    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(DxMesh(&mesh));
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
            Logger::LogError("DirectX: " + result, true);
            return false;
        }
    }

    for(DxMesh& mesh : m_data->meshes)
    {
        mesh.Initialise(m_data->device, m_data->context);
    }

    for(DxTexture& texture : m_data->textures)
    {
        texture.Initialise(m_data->device);
    }

    return true;
}

void DirectxEngine::BeginRender()
{
    m_data->selectedShader = NO_INDEX; // always reset due to post shader
    m_data->sceneTarget.SetActive(m_data->context, m_data->zbuffer);
}

void DirectxEngine::Render(const std::vector<Light>& lights)
{
    m_data->lightsUpdated = true; // updated once per tick due to tweaking

    for(DxMesh& mesh : m_data->meshes)
    {
        UpdateShader(mesh.GetShaderID(), lights);
        SetTextures(mesh.GetTextureIDs());
        SetBackfaceCull(mesh.ShouldBackfaceCull());
        mesh.Render(m_data->context);
    }

    // Render the scene as a texture to the backbuffer
    m_data->backBuffer.SetActive(m_data->context);
    m_data->postShader.SetActive(m_data->context);
    m_data->sceneTarget.SendTexture(m_data->context, 0);
    m_data->quad.Render(m_data->context);
    m_data->sceneTarget.ClearTexture(m_data->context, 0);
}

void DirectxEngine::SetTextures(const std::vector<int>& textureIDs)
{
    DxShader& shader = m_data->shaders[m_data->selectedShader];

    int slot = 0;
    for(int id : textureIDs)
    {
        if(id != NO_INDEX)
        {
            if(shader.HasTextureSlot(slot))
            {
                m_data->textures[id].SendTexture(m_data->context, slot++);
            }
            else
            {
                Logger::LogError("Shader and mesh texture count does not match");
            }
        }
    }
}

void DirectxEngine::UpdateShader(int index, const std::vector<Light>& lights)
{
    bool updatedConstants = false;
    bool changedShader = false;
    if(index != m_data->selectedShader)
    {
        m_data->shaders[index].SetActive(m_data->context);
        m_data->selectedShader = index;
        changedShader = true;
    }
    DxShader& shader = m_data->shaders[m_data->selectedShader];

    // Update transform information
    if(changedShader || m_data->viewUpdated)
    {
        // Model pivot points exist at the origin: world matrix is the identity
        shader.UpdateConstantMatrix("viewProjection", m_data->view * m_data->projection);
        shader.UpdateConstantFloat("cameraPosition", &m_data->camera.x, 3);

        m_data->viewUpdated = false;
        updatedConstants = true;
    }

    // Update light information
    if(changedShader || m_data->lightsUpdated)
    {
        shader.UpdateConstantFloat("lightPosition", &lights[0].position.x, 3);
        m_data->lightsUpdated = false;
        updatedConstants = true;
    }

    if(updatedConstants)
    {
        shader.SendConstants(m_data->context);
    }
}

void DirectxEngine::EndRender()
{
    m_data->swapchain->Present(0, 0);
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

    m_data->camera.x = world.m14;
    m_data->camera.y = world.m24;
    m_data->camera.z = world.m34;

    m_data->viewUpdated = true;
    D3DXMatrixInverse(&m_data->view, nullptr, &m_data->view);
}

void DirectxEngine::SetBackfaceCull(bool shouldCull)
{
    if(shouldCull != m_data->isBackfaceCull)
    {
        m_data->isBackfaceCull = shouldCull;
        m_data->context->RSSetState(shouldCull ? m_data->cullState : m_data->nocullState);
    }
}