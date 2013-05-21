/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Shader base class
*****************************************************************/
#pragma once

#include <boost/noncopyable.hpp>
#include "common.h"

class Shader : public IShaderConstantSetCallBack, private boost::noncopyable
{
public:

    Shader();
    virtual ~Shader();

    /**
    * Initialises the shader
    * @param the name of the shader
    * @param whether the shader uses transparency
    * @param whether or not the shader uses multiple lights
    * @param optional path to find the shader
    * @return whether or not initialisation succeeded
    */
    bool InitialiseShader(const std::string& name, bool usesAlpha, bool usesMultipleLights, const std::string& path = "");

    /**
    * @return the name of the shader
    */
    const std::string& GetName() const { return m_name; }

    /**
    * Sets the constants for the shader for a particular mesh
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * Get the internal material index of the shader
    */
    int GetMaterialIndex() const { return m_materialIndex; }

protected:

    static const std::string sm_shaderPath;       ///< Path for all base shaders
    static const std::string sm_generatedFolder;  ///< Folder for all generated shaders
    static const std::string sm_vertexExt;        ///< Extension for the vertex shader
    static const std::string sm_fragmentExt;      ///< Extension for the fragment shader

    int m_materialIndex;   ///< The irrlicht index representing the shader
    std::string m_name;    ///< The name of the shader
};