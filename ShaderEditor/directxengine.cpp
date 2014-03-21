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
};

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    context(nullptr),
    backbuffer(nullptr)
{
}

DirectxData::~DirectxData()
{
    Release();
}

void DirectxData::Release()
{
    for(DxShader& shader : shaders)
    {
        shader.Release();
    }

    if(swapchain)
    {
        swapchain->Release();
        swapchain = nullptr;
    }
    if(device)
    {
        device->Release();
        device = nullptr;
    }
    if(context)
    {
        context->Release();
        context = nullptr;
    }
    if(backbuffer)
    {
        backbuffer->Release();
        backbuffer = nullptr;
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

    if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_data->swapchain,
        &m_data->device, nullptr, &m_data->context)))
    {
        Logger::LogError("DirectX: Device creation failed");
        return false;
    }

    // Set the back buffer as the main render target
    ID3D11Texture2D* backBuffer;
    m_data->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    m_data->device->CreateRenderTargetView(backBuffer, nullptr, &m_data->backbuffer);
    m_data->context->OMSetRenderTargets(1, &m_data->backbuffer, nullptr);
    backBuffer->Release();

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;
    m_data->context->RSSetViewports(1, &viewport);

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
        m_data->shaders.push_back(DxShader(shader.hlslShaderFile));
    }

    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(DxMesh(m_data->device, m_data->context, mesh));
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