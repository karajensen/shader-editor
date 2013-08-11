////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#include "shader.h"
#include <boost/algorithm/string.hpp>

namespace 
{
    const std::string FRAGMENT_EXTENSION = ".frag";          
    const std::string VERTEX_EXTENSION = ".vert";            
    const std::string SHADER_PATH = ASSETS_PATH+"Shaders//";
}

Shader::Shader(EnginePtr engine) :
    m_materialIndex(NO_INDEX),
    m_engine(engine)
{
}

Shader::~Shader()
{
}

void Shader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    IVideoDriver* driver = m_engine->driver;

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

bool Shader::InitialiseShader(const std::string& name, bool usesAlpha, 
    bool usesMultipleLights, const std::string& path)
{
    m_name = boost::to_lower_copy(name);
    IGPUProgrammingServices* gpu = m_engine->driver->getGPUProgrammingServices();

    if(gpu)
    {
        video::E_MATERIAL_TYPE materialType = 
            usesAlpha ? video::EMT_TRANSPARENT_ADD_COLOR : video::EMT_SOLID;

        std::string filepath((path.empty() ? SHADER_PATH : path)+name);

        m_materialIndex = gpu->addHighLevelShaderMaterialFromFiles(
            (filepath+VERTEX_EXTENSION).c_str(), "main", video::EVST_VS_5_0,
            (filepath+FRAGMENT_EXTENSION).c_str(), "main", video::EPST_PS_5_0,
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