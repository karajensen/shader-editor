#include "shader.h"
#include <boost/algorithm/string.hpp>

const std::string Shader::sm_fragmentExt(".frag");
const std::string Shader::sm_vertexExt(".vert");
const std::string Shader::sm_shaderPath(ASSETS_PATH+"Shaders//");

Shader::Shader() :
    m_materialIndex(NO_INDEX)
{
}

Shader::~Shader()
{
}

void Shader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    IVideoDriver* driver = Driver();

    // set inverted world matrix   
    core::matrix4 world = driver->getTransform(video::ETS_WORLD);
    core::matrix4 invWorld = world;
    invWorld.makeInverse();
    services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
    
    // set world view proj matrix
    core::matrix4 worldViewProj;
    worldViewProj = driver->getTransform(video::ETS_PROJECTION);
    worldViewProj *= driver->getTransform(video::ETS_VIEW);
    worldViewProj *= world;
    services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

    // set transposed world matrix
    world = world.getTransposed();
    services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

    // set textures
    s32 TextureLayerID = 0;
    services->setPixelShaderConstant("Sampler0", &TextureLayerID, 1);

    TextureLayerID = 1;
    services->setPixelShaderConstant("Sampler1", &TextureLayerID, 1);

    TextureLayerID = 2;
    services->setPixelShaderConstant("Sampler2", &TextureLayerID, 1);

    TextureLayerID = 3;
    services->setPixelShaderConstant("Sampler3", &TextureLayerID, 1);

    TextureLayerID = 4;
    services->setPixelShaderConstant("Sampler4", &TextureLayerID, 1);
}

bool Shader::InitialiseShader(const std::string& name, bool usesAlpha, bool usesMultipleLights, const std::string& path)
{
    m_name = boost::to_lower_copy(name);
    IGPUProgrammingServices* gpu = Driver()->getGPUProgrammingServices();

    if(gpu)
    {
        video::E_MATERIAL_TYPE materialType = 
            usesAlpha ? video::EMT_TRANSPARENT_ADD_COLOR : video::EMT_SOLID;

        std::string filepath((path.empty() ? sm_shaderPath : path)+name);

        m_materialIndex = gpu->addHighLevelShaderMaterialFromFiles(
            (filepath+sm_vertexExt).c_str(), "main", video::EVST_VS_5_0,
            (filepath+sm_fragmentExt).c_str(), "main", video::EPST_PS_5_0,
            this, materialType, 0, video::EGSL_DEFAULT);

        if(m_materialIndex == NO_INDEX)
        {
            Logger::LogError(name + " Shader Failed to initialise");
            return false;
        }
    }
    else
    {
        Logger::LogError("getGPUProgrammingServices() failed");
        return false;
    }
    return true;
}