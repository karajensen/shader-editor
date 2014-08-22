////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxengine.h"
#include "directxcommon.h"
#include "directxshader.h"
#include "directxmesh.h"
#include "directxtexture.h"
#include "directxtarget.h"
#include <array>

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

    ID3D11RasterizerState* cullState = nullptr;   ///< Normal state of the rasterizer
    ID3D11RasterizerState* nocullState = nullptr; ///< No face culling state of the rasterizer
    IDXGISwapChain* swapchain = nullptr;          ///< Collection of buffers for displaying frames
    ID3D11Device* device = nullptr;               ///< Direct3D device interface
    ID3D11DeviceContext* context = nullptr;       ///< Direct3D device context
    ID3D11Debug* debug = nullptr;                 ///< Direct3D debug interface, only created in debug

    DxShader normalShader;              ///< Shader for rendering normals/depth for the scene
    DxShader postShader;                ///< Post processing shader
    DxMesh quad;                        ///< Quad to render the final post processed scene onto
    DxRenderTarget backBuffer;          ///< Render target for the back buffer
    DxRenderTarget sceneTarget;         ///< Render target for the main scene
    DxRenderTarget normalTarget;        ///< Render target for the scene normal/depth map
    D3DXMATRIX view;                    ///< View matrix
    D3DXMATRIX projection;              ///< Projection matrix
    D3DXMATRIX viewProjection;          ///< View projection matrix
    D3DXVECTOR3 cameraPosition;         ///< Position of the camera
    D3DXVECTOR2 cameraDepth;            ///< Camera near and far values
    bool isBackfaceCull = true;         ///< Whether the culling rasterize state is active
    int selectedShader = NO_INDEX;      ///< currently selected shader for rendering the scene
    float fadeAmount = 0.0f;            ///< the amount to fade the scene by
    
    std::array<float, Texture::MAX_POST> postAlpha;   ///< Visibility of post textures
    std::vector<std::unique_ptr<DxTexture>> textures; ///< Textures shared by all meshes
    std::vector<std::unique_ptr<DxMesh>> meshes;      ///< Each mesh in the scene
    std::vector<std::unique_ptr<DxShader>> shaders;   ///< Shaders shared by all meshes
};

DirectxData::DirectxData() :
    sceneTarget("SceneTarget"),
    normalTarget("NormalTarget"),
    backBuffer("BackBuffer", true),
    cameraDepth(DEPTH_NEAR, DEPTH_FAR),
    postShader(POST_NAME, POST_PATH),
    normalShader(NORMAL_NAME, NORM_PATH),
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
    isBackfaceCull = true;
    fadeAmount = 0.0f;

    for(auto& texture : textures)
    {
        texture->Release();
    }

    for(auto& mesh : meshes)
    {
        mesh->Release();
    }

    for(auto& shader : shaders)
    {
        shader->Release();
    }

    normalShader.Release();
    postShader.Release();
    quad.Release();
    sceneTarget.Release();
    normalTarget.Release();
    backBuffer.Release();

    SafeRelease(&cullState);
    SafeRelease(&nocullState);
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
       !m_data->normalTarget.Initialise(m_data->device))
    {
        Logger::LogError("DirectX: Failed to create render targets");
        return false;
    }

    // Create the normal/depth shader
    std::string errorBuffer = m_data->normalShader.CompileShader(m_data->device);
    if(!errorBuffer.empty())
    {
        Logger::LogError("DirectX: Normal shader failed: " + errorBuffer);
        return false;
    }

    // Create the post processing quad and shader
    m_data->quad.Initialise(m_data->device, m_data->context);
    errorBuffer = m_data->postShader.CompileShader(m_data->device);
    if (!errorBuffer.empty())
    {
        Logger::LogError("DirectX: Post shader failed: " + errorBuffer);
        return false;
    }

    if (!m_data->postShader.HasTextureSlot(Texture::SCENE_TEXTURE) ||
        !m_data->postShader.HasTextureSlot(Texture::NORMAL_TEXTURE))
    {
        Logger::LogError("DirectX: Post shader does not have required texture slots");
        return false;
    }

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
        FRUSTRUM_NEAR, FRUSTRUM_FAR);

    SetDebugName(m_data->cullState, "CullState");
    SetDebugName(m_data->nocullState, "NoCullState");
    SetDebugName(m_data->device, "Device");
    SetDebugName(m_data->context, "Context");
    SetDebugName(m_data->swapchain, "SwapChain");

    SetPostTexture(Texture::SCENE_TEXTURE);

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

bool DirectxEngine::InitialiseScene(const std::vector<Mesh>& meshes, 
                                    const std::vector<Mesh>& alpha, 
                                    const std::vector<Shader>& shaders,
                                    const std::vector<Texture>& textures)
{
    m_data->textures.reserve(textures.size());
    for(const Texture& texture : textures)
    {
        m_data->textures.push_back(std::unique_ptr<DxTexture>(
            new DxTexture(texture.path)));
    }

    m_data->shaders.reserve(shaders.size());
    for(const Shader& shader : shaders)
    {
        m_data->shaders.push_back(std::unique_ptr<DxShader>(
            new DxShader(shader.name, shader.hlslShaderFile, shader.index)));
    }

    m_data->meshes.reserve(meshes.size());
    for(const Mesh& mesh : meshes)
    {
        m_data->meshes.push_back(std::unique_ptr<DxMesh>(
            new DxMesh(&mesh)));
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

    for(auto& texture : m_data->textures)
    {
        texture->Initialise(m_data->device);
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

void DirectxEngine::Render(const std::vector<Light>& lights)
{
    m_data->selectedShader = NO_INDEX; // always reset due to post shader

    // Render the scene
    m_data->sceneTarget.SetActive(m_data->context);
    for(auto& mesh : m_data->meshes)
    {
        UpdateShader(mesh->GetMesh(), lights);

        SetTextures(mesh->GetTextureIDs());
        SetBackfaceCull(mesh->ShouldBackfaceCull());
        
        mesh->Render(m_data->context);
    }

    // Render the normal/depth map
    m_data->normalTarget.SetActive(m_data->context);
    m_data->normalShader.SetActive(m_data->context);
    m_data->normalShader.UpdateConstantMatrix("viewProjection", m_data->viewProjection);
    m_data->normalShader.UpdateConstantFloat("cameraDepth", &m_data->cameraDepth.x, 2);
    m_data->normalShader.SendConstants(m_data->context);
    for(auto& mesh : m_data->meshes)
    {
        SetBackfaceCull(mesh->ShouldBackfaceCull());
        mesh->Render(m_data->context);
    }

    // Render the scene as a texture to the backbuffer
    SetBackfaceCull(false);
    m_data->backBuffer.SetActive(m_data->context);
    m_data->postShader.SetActive(m_data->context);
    RenderPostProcessing();

    m_data->swapchain->Present(0, 0);
}

void DirectxEngine::RenderPostProcessing()
{
    m_data->sceneTarget.SendTexture(m_data->context, Texture::SCENE_TEXTURE);
    m_data->normalTarget.SendTexture(m_data->context, Texture::NORMAL_TEXTURE);

    m_data->postShader.UpdateConstantFloat("fadeAmount",
        &m_data->fadeAmount, 1);

    m_data->postShader.UpdateConstantFloat("sceneAlpha",
        &m_data->postAlpha[Texture::SCENE_TEXTURE], 1);

    m_data->postShader.UpdateConstantFloat("normalAlpha",
        &m_data->postAlpha[Texture::NORMAL_TEXTURE], 1);

    m_data->postShader.UpdateConstantFloat("depthAlpha",
        &m_data->postAlpha[Texture::DEPTH_TEXTURE], 1);

    m_data->postShader.SendConstants(m_data->context);

    m_data->quad.Render(m_data->context);

    m_data->sceneTarget.ClearTexture(m_data->context, Texture::SCENE_TEXTURE);
    m_data->normalTarget.ClearTexture(m_data->context, Texture::NORMAL_TEXTURE);
}

void DirectxEngine::SetTextures(const std::vector<int>& textureIDs)
{
    auto& shader = m_data->shaders[m_data->selectedShader];

    int slot = 0;
    for(int id : textureIDs)
    {
        if(id != NO_INDEX)
        {
            if(shader->HasTextureSlot(slot))
            {
                m_data->textures[id]->SendTexture(m_data->context, slot++);
            }
            else
            {
                Logger::LogError("Shader and mesh texture count does not match");
            }
        }
    }
}

void DirectxEngine::UpdateShader(const Mesh& mesh, 
                                 const std::vector<Light>& lights)
{
    const int index = mesh.shaderIndex;
    auto& shader = m_data->shaders[index];

    if(index != m_data->selectedShader)
    {
        m_data->selectedShader = index;
        shader->SetActive(m_data->context);
        
        shader->UpdateConstantMatrix("viewProjection", m_data->viewProjection);
        shader->UpdateConstantFloat("cameraPosition", &m_data->cameraPosition.x, 3);

        shader->UpdateConstantFloat("lightSpecularity", &lights[0].specularity, 1);
        shader->UpdateConstantFloat("lightAttenuation", &lights[0].attenuation.x, 3);
        shader->UpdateConstantFloat("lightPosition", &lights[0].position.x, 3);
        shader->UpdateConstantFloat("lightDiffuse", &lights[0].diffuse.r, 3);
        shader->UpdateConstantFloat("lightSpecular", &lights[0].specular.r, 3);
    }

    shader->UpdateConstantFloat("meshAmbience", &mesh.ambience, 1);
    shader->UpdateConstantFloat("meshBump", &mesh.bump, 1);
    shader->UpdateConstantFloat("meshSpecularity", &mesh.specularity, 1);
    shader->SendConstants(m_data->context);
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

    m_data->cameraPosition.x = world.m14;
    m_data->cameraPosition.y = world.m24;
    m_data->cameraPosition.z = world.m34;

    D3DXMatrixInverse(&m_data->view, nullptr, &m_data->view);

    // Model pivot points exist at the origin: world matrix is the identity
    m_data->viewProjection = m_data->view * m_data->projection;
}

void DirectxEngine::SetBackfaceCull(bool shouldCull)
{
    if(shouldCull != m_data->isBackfaceCull)
    {
        m_data->isBackfaceCull = shouldCull;
        m_data->context->RSSetState(shouldCull ? m_data->cullState : m_data->nocullState);
    }
}

std::string DirectxEngine::GetShaderText(int index) const
{
    if(index == m_data->shaders.size())
    {
        return m_data->postShader.GetText();
    }
    else if(index == m_data->shaders.size() + 1)
    {
        return m_data->normalShader.GetText();
    }
    return m_data->shaders[index]->GetText();
}

std::string DirectxEngine::GetShaderAssembly(int index)
{
    if(index == m_data->shaders.size())
    {
        return m_data->postShader.GetAssembly();
    }
    else if(index == m_data->shaders.size() + 1)
    {
        return m_data->normalShader.GetAssembly();
    }
    return m_data->shaders[index]->GetAssembly();
}

void DirectxEngine::SetFade(float value)
{
    m_data->fadeAmount = value;
}

void DirectxEngine::SetPostTexture(Texture::Post post)
{
    m_data->postAlpha.assign(0.0f);
    m_data->postAlpha[post] = 1.0f;
}

void DirectxEngine::SetDepthNear(float value)
{
    m_data->cameraDepth.x = value;
}

void DirectxEngine::SetDepthFar(float value)
{
    m_data->cameraDepth.y = value;
}