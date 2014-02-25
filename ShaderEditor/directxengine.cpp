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
* Holds information for an individual directx shader
*/
struct DxShader
{
    /**
    * Constructor
    */
    DxShader();

    /**
    * Destructor
    */
    ~DxShader();

    /**
    * Releases the shader object
    */
    void Release();

    std::string filepath;      // Path to the shader file
    ID3D11InputLayout* layout; // Shader input layout
    ID3D11VertexShader* vs;    // HLSL vertex shader
    ID3D11PixelShader* ps;     // HLSL pixel shader
};

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
            
    std::vector<DxShader> shaders;        ///< DirectX shader objects
    ID3D11RenderTargetView* backbuffer;   ///< Main back buffer render target
    IDXGISwapChain* swapchain;            ///< swap chain interface
    ID3D11Device* device;                 ///< Direct3D device interface
    ID3D11DeviceContext* context;         ///< Direct3D device context
};

DxShader::DxShader() :
    layout(nullptr),
    vs(nullptr),
    ps(nullptr)
{
}

DirectxData::DirectxData() :
    swapchain(nullptr),
    device(nullptr),
    context(nullptr),
    backbuffer(nullptr)
{
}

DirectxEngine::DirectxEngine(HWND hwnd) :
    m_data(new DirectxData()),
    m_hwnd(hwnd)
{
}

DxShader::~DxShader()
{
    Release();
}

DirectxData::~DirectxData()
{
    Release();
}

DirectxEngine::~DirectxEngine()
{
}

void DxShader::Release()
{
    if(layout)
    {
        layout->Release();
        layout = nullptr;
    }
    if(vs)
    {
        vs->Release();
        vs = nullptr;
    }
    if(ps)
    {
        ps->Release();
        ps = nullptr;
    }
}

void DirectxData::Release()
{
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
    DxShader& shader = m_data->shaders[index];
    ID3D10Blob* vsBlob = nullptr;
    ID3D10Blob* psBlob = nullptr;
    ID3D10Blob* errors = nullptr;

    if(FAILED(D3DX11CompileFromFile(shader.filepath.c_str(), 0, 0,
        "VShader", "vs_5_0", 0, 0, 0, &vsBlob, &errors, 0)))
    {
        return errors ? static_cast<char*>(errors->GetBufferPointer()) : 
            std::string("Unknown Error in Vertex Shader").c_str();
    }

    if(FAILED(D3DX11CompileFromFile(shader.filepath.c_str(), 0, 0, 
        "PShader", "ps_5_0", 0, 0, 0, &psBlob, &errors, 0)))
    {
        return errors ? static_cast<char*>(errors->GetBufferPointer()) : 
            std::string("Unknown Error in Pixel Shader").c_str();
    }

    shader.Release();

    m_data->device->CreateVertexShader(vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(), 0, &shader.vs);
    
    m_data->device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(), 0, &shader.ps);
    
    D3D11_INPUT_ELEMENT_DESC vertexDescription[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    if(FAILED(m_data->device->CreateInputLayout(vertexDescription, 2, 
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &shader.layout)))
    {
        return "DirectX: Could not create input layout";
    }

    return "";
}

bool DirectxEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                    const std::vector<Mesh>& alpha, 
                                    const std::vector<Shader>& shaders)
{
    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        const int index = m_data->shaders.size();
        m_data->shaders.push_back(DxShader());
        m_data->shaders[index].filepath = shader.hlslFragmentFile;
        const std::string result = CompileShader(index);
        if(!result.empty())
        {
            Logger::LogError("DirectX: " + result);
            return false;
        }
    }
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

void DirectxEngine::BeginRender()
{
    m_data->context->ClearRenderTargetView(
        m_data->backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
}

void DirectxEngine::Render(const std::vector<Light>& lights)
{
    //m_data->context->VSSetShader(shader.vs, 0, 0);
    //m_data->context->PSSetShader(shader.ps, 0, 0);
    //m_data->context->IASetInputLayout(shader.layout);
}

void DirectxEngine::EndRender()
{
    m_data->swapchain->Present(0, 0);
}