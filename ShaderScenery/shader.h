////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/noncopyable.hpp>
#include "common.h"

/**
* Irrlicht Shader base class
*/
class Shader : public IShaderConstantSetCallBack, private boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit Shader(EnginePtr engine);

    /**
    * Destructor
    */
    virtual ~Shader();

    /**
    * Initialises the shader
    * @param name The name of the shader
    * @param usesAlpha Whether the shader uses transparency
    * @param usesMultipleLights Whether or not the shader uses multiple lights
    * @param path Optional path to find the shader
    * @return whether or not initialisation succeeded
    */
    bool InitialiseShader(const std::string& name, bool usesAlpha, 
        bool usesMultipleLights, const std::string& path = "");

    /**
    * @return the name of the shader
    */
    const std::string& GetName() const { return m_name; }

    /**
    * Sets the constants for the shader
    * @param services Interface providing methods for communicating with shaders
    * @param userData User defined int specific on shader creation
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * @return The internal material index of the shader
    */
    int GetMaterialIndex() const { return m_materialIndex; }

protected: 

    int m_materialIndex;   ///< The irrlicht index representing the shader
    std::string m_name;    ///< The name of the shader
    EnginePtr m_engine;    ///< Irrlicht engine
};