////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxcommon.h"
#include "directxshader.h"
#include "directxmesh.h"

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
            
    std::vector<DxMesh> meshes;           ///< DirectX mesh objects
    std::vector<DxShader> shaders;        ///< DirectX shader objects
    ID3D11RenderTargetView* backbuffer;   ///< Main back buffer render target
    IDXGISwapChain* swapchain;            ///< swap chain interface
    ID3D11Device* device;                 ///< Direct3D device interface
    ID3D11DeviceContext* context;         ///< Direct3D device context
    ID3D11Debug* debug;                   ///< Direct3D debug interface
    D3DXMATRIX view;                      ///< View matrix
    D3DXMATRIX projection;                ///< Projection matrix
};

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    context(nullptr),
    backbuffer(nullptr),
    debug(nullptr)
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
    for(DxMesh& mesh : meshes)
    {
        mesh.Release();
    }

    for(DxShader& shader : shaders)
    {
        shader.Release();
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
    
    #ifdef _DEBUG
    unsigned int deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
    #elif
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

    // Set the back buffer as the main render target
    ID3D11Texture2D* backBuffer;
    m_data->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    m_data->device->CreateRenderTargetView(backBuffer, nullptr, &m_data->backbuffer);
    m_data->context->OMSetRenderTargets(1, &m_data->backbuffer, nullptr);
    backBuffer->Release();

    // Setup the directX environment
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;
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
                                    const std::vector<Shader>& shaders)
{
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
            Logger::LogError("DirectX: " + result);
            return false;
        }
    }

    for(DxMesh& mesh : m_data->meshes)
    {
        mesh.Initialise(m_data->device, m_data->context);
    }

    return true;
}

void DirectxEngine::BeginRender()
{
    m_data->context->ClearRenderTargetView(
        m_data->backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
}

void DirectxEngine::Render(const std::vector<Light>& lights)
{
    DxShader& shader = m_data->shaders[0];
    shader.SetAsActive(m_data->context);
    
    const D3DXMATRIX viewProj = m_data->view * m_data->projection;
    shader.UpdateConstantMatrix("viewProjection", viewProj);
    
    const float testing = 0.2f;
    shader.UpdateConstantFloat("testing", testing);
    
    shader.SendConstants(m_data->context);
    
    for(DxMesh& mesh : m_data->meshes)
    {
        mesh.Render(m_data->context);
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

    D3DXMatrixInverse(&m_data->view, nullptr, &m_data->view);
}