/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Shader class, constructs a shader from components 
* or loads directly from file
*****************************************************************/
#pragma once

#include <iostream>
#include <boost/noncopyable.hpp>
#include "common.h"

class Shader : public IShaderConstantSetCallBack
{
public:

    ~Shader();
    Shader();

    /**
    * Initialises the shader
    * @param the name of the shader
    * @param whether the shader uses transparency
    * @param whether or not the shader uses multiple lights
    * @return whether or not initialisation succeeded
    */
    bool InitialiseShader(const std::string& name, bool usesAlpha, bool usesMultipleLights);

    /**
    * Initialises the shader from fragments
    * @param the name of the shader
    * @param whether or not the shader uses multiple lights
    * @return whether or not initialisation succeeded
    */
    bool InitialiseFromFragments(const std::string& name, bool usesMultipleLights);

    /**
    * @return the name of the shader
    */
    const std::string& GetName() const { return m_name; }

    /**
    * Sets the constants for the shader for a particular mesh
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);

    /**
    * Get the internal material index of the shader
    */
    int GetMaterialIndex() const { return m_materialIndex; }

    /**
    * Retrieves a vector of shader component names
    */
    static std::vector<std::string> GetShaderComponents();

    /**
    * Avaliable texture slots
    */
    enum TextureSlots
    {
        TextureSlot0,
        TextureSlot1,
        TextureSlot2,
        TextureSlot3,
        TextureSlot4
    };

    /**
    * Avaliable Shader Components
    */
    static const std::string FLAT;
    static const std::string BUMP;
    static const std::string SPECULAR;
    static const std::string ALPHA;
    static const std::string PARALLAX;

private:

    /**
    * Does custom pre-processing of the shader files before sending them to irrlicht
    * This allows shaders to be constructed from shader fragments
    * @param the name of the shader
    * @param whether or not this shader is the vertex shader
    * @param whether or not this shader uses multiple lights
    * @return whether or not the generation failed
    */
    bool CreateShaderFromFragments(const std::string& name, bool isVertex, bool usesMultipleLights);

    /**
    * Reads from a particular file and adds lines to the new shader file
    * @param the path of the file to open and read from
    * @param the assets path of all shaders
    * @param the new file to add lines from the opened file
    * @return whether the call succeeded
    */
    bool ReadFromFile(const std::string& filepath, const std::string& assetspath, std::ofstream& newfile);

    /**
    * Adds the shader component to the list of components if the name allows it
    * @param the component to add
    * @param the name of the shader
    */
    void AddShaderComponent(const std::string& component,const std::string& name);

    /**
    * Adds the shader component to the list of components
    * @param the component to add
    */
    void AddShaderComponent(const std::string& component);

    /**
    * Reads the file until a specific string is found
    * @param the assets path of all shaders
    * @param the file to read
    * @param the target string to read until
    * @param the new file to add lines from the opened file
    * @Param whether to write to the new file or not
    * @return the last line read after the target string is found
    */
    typedef std::vector<std::string> TargetVector;
    std::string ReadFile(const std::string& assetspath, std::ifstream& file, std::ofstream& newfile, const TargetVector& target, bool skiplines);

    std::vector<std::string> m_shaderComponent; ///< Components part of this shader
    int m_materialIndex; ///< The irrlicht index representing the shader
    std::string m_name;  ///< The name of the shader
    bool m_generated;    ///< Whether the shader was generated from fragments
};