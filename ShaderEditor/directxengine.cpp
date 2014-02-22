////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "common.h"

#pragma warning (disable : 4005)
#include "directx/include/d3d11.h"
#include "directx/include/d3dx11.h"
#include "directx/include/d3dx10.h"

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

    ID3D11RenderTargetView* backbuffer;  ///< Main back buffer render target
    IDXGISwapChain* swapchain;           ///< swap chain interface
    ID3D11Device* device;                ///< Direct3D device interface
    ID3D11DeviceContext* devcon;         ///< Direct3D device context
};

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    devcon(nullptr),
    backbuffer(nullptr)
{
}

DirectxData::~DirectxData()
{
    if(swapchain)
    {
        swapchain->Release();
    }
    if(device)
    {
        device->Release();
    }
    if(devcon)
    {
        devcon->Release();
    }
    if(backbuffer)
    {
        backbuffer->Release();
    }
}

DirectxEngine::DirectxEngine(HWND hwnd) :
    m_hwnd(hwnd)
{
}

DirectxEngine::~DirectxEngine()
{
}

bool DirectxEngine::Initialize()
{
    m_data.reset(new DirectxData());

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
        &m_data->device, nullptr, &m_data->devcon)))
    {
        Logger::LogError("DirectX: Device creation failed");
        return false;
    }

    // Set the back buffer as the main render target
    ID3D11Texture2D* backBuffer;
    m_data->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    m_data->device->CreateRenderTargetView(backBuffer, nullptr, &m_data->backbuffer);
    m_data->devcon->OMSetRenderTargets(1, &m_data->backbuffer, nullptr);
    backBuffer->Release();

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;
    m_data->devcon->RSSetViewports(1, &viewport);

    Logger::LogInfo("DirectX: D3D11 sucessful");
    return true;
}

void DirectxEngine::BeginRender()
{
    m_data->devcon->ClearRenderTargetView(
        m_data->backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
}

void DirectxEngine::EndRender()
{
    m_data->swapchain->Present(0, 0);
}

void DirectxEngine::CompileShader(int index)
{


}

bool DirectxEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                   const std::vector<Mesh>& alpha, 
                                   const std::vector<Shader>& shaders)
{

    return true;
}

ID3D11Device* DirectxEngine::GetDevice() const
{
    return m_data->device;
}

std::string DirectxEngine::GetName() const
{
    return "DirectX";
}