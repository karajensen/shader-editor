////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxcommon.h"
#include "directxshader.h"
#include "directxmesh.h"
#include "directxtexture.h"

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
            
    std::vector<DxTexture> textures;      ///< DirectX texture objects
    std::vector<DxMesh> meshes;           ///< DirectX mesh objects
    std::vector<DxShader> shaders;        ///< DirectX shader objects
    ID3D11DepthStencilView* zbuffer;      ///< Depth buffer
    ID3D11RenderTargetView* backbuffer;   ///< Main back buffer render target
    IDXGISwapChain* swapchain;            ///< swap chain interface
    ID3D11Device* device;                 ///< Direct3D device interface
    ID3D11DeviceContext* context;         ///< Direct3D device context
    ID3D11Debug* debug;                   ///< Direct3D debug interface
    D3DXMATRIX view;                      ///< View matrix
    D3DXMATRIX projection;                ///< Projection matrix
    bool isBackfaceCull;                  ///< Whether the culling rasterize state is active
    ID3D11RasterizerState* cullState;     ///< Normal state of the rasterizer
    ID3D11RasterizerState* nocullState;   ///< No face culling state of the rasterizer
    int selectedShader;                   ///< currently selected shader
    bool viewUpdated;                     ///< Whether the view matrix has updated this tick
    bool lightsUpdated;                   ///< Whether the lights have been updated this tick
};

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    context(nullptr),
    backbuffer(nullptr),
    debug(nullptr),
    zbuffer(nullptr),
    cullState(nullptr),
    nocullState(nullptr),
    isBackfaceCull(true),
    selectedShader(NO_INDEX),
    viewUpdated(true),
    lightsUpdated(true)
{
}

DirectxData::~DirectxData()
{
    Release();
    if(debug)
    {
        debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        debug->Release();
        debug = nullptr;
    }
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

    if(cullState)
    {
        cullState->Release();
        cullState = nullptr;
    }

    if(nocullState)
    {
        nocullState->Release();
        nocullState = nullptr;
    }

    if(zbuffer)
    {
        zbuffer->Release();
        zbuffer = nullptr;
    }

    if(swapchain)
    {
        swapchain->Release();
        swapchain = nullptr;
    }

    if(backbuffer)
    {
        backbuffer->Release();
        backbuffer = nullptr;
    }

    if(context)
    {
        context->Release();
        context = nullptr;
    }

    if(device)
    {
        device->Release();
        device = nullptr;
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
    scd.SampleDesc.Count = 4;                           
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

    // Set to throw if there is any severe problem with directX
    #ifdef _DEBUG
    if(SUCCEEDED(m_data->device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_data->debug)))
    {
        ID3D11InfoQueue *d3dInfoQueue = nullptr;
        if(SUCCEEDED(m_data->debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
        }
    }
    #endif

    // Create the depth texture
    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = WINDOW_WIDTH;
    depthTextureDesc.Height = WINDOW_HEIGHT;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.SampleDesc.Count = MULTISAMPLING_COUNT;
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
    depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthBufferDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    if(FAILED(m_data->device->CreateDepthStencilView(
        depthTexture, &depthBufferDesc, &m_data->zbuffer)))
    {
        Logger::LogError("DirectX: Depth buffer creation failed");
        return false;
    }
    depthTexture->Release();

    // Set the back buffer as the main render target
    ID3D11Texture2D* backBuffer;
    m_data->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if(FAILED(m_data->device->CreateRenderTargetView(backBuffer, nullptr, &m_data->backbuffer)))
    {
        Logger::LogError("DirectX: Failed to create back buffer render target");
        return false;
    }
    m_data->context->OMSetRenderTargets(1, &m_data->backbuffer, m_data->zbuffer);
    backBuffer->Release();

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

    Logger::LogInfo("DirectX: D3D11 sucessful");
    return true;
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
        m_data->meshes.push_back(DxMesh(mesh));
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
    m_data->context->ClearRenderTargetView(
        m_data->backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    m_data->context->ClearDepthStencilView(
        m_data->zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectxEngine::Render(const std::vector<Light>& lights)
{
    m_data->lightsUpdated = true; // updated once per tick due to tweaking

    for(DxMesh& mesh : m_data->meshes)
    {
        UpdateShader(mesh.GetShaderID(), lights);

        // Send texture information
        int slot = 0;
        for(int id : mesh.GetTextureIDs())
        {
            if(id != NO_INDEX)
            {
                m_data->textures[id].SendTexture(m_data->context, slot++);
            }
        }

        // Render the mesh
        SetBackfaceCull(mesh.ShouldBackfaceCull());
        mesh.Render(m_data->context);
    }
}

void DirectxEngine::UpdateShader(int index, const std::vector<Light>& lights)
{
    bool updatedConstants = false;
    bool changedShader = false;
    if(index != m_data->selectedShader)
    {
        m_data->shaders[index].SetAsActive(m_data->context);
        m_data->selectedShader = index;
        changedShader = true;
    }
    DxShader& shader = m_data->shaders[m_data->selectedShader];

    // Update transform information
    if(changedShader || m_data->viewUpdated)
    {
        // Model pivot points exist at the origin: world matrix is the identity
        shader.UpdateConstantMatrix("viewProjection", m_data->view * m_data->projection);
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