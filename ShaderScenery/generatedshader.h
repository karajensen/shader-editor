////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - generatedshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <fstream>
#include "shader.h"

class ShaderEditor;

/**
* Fragment linker to auto-generate shaders for each mesh
*/
class GeneratedShader : public Shader
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    * @param editor Shader editor for handling shader components
    */
    GeneratedShader(EnginePtr engine, boost::shared_ptr<ShaderEditor> editor);

    /**
    * Destructor
    */
    ~GeneratedShader();

    /**
    * Initialises the shader from fragments
    * @param name The name of the shader
    * @param usesMultipleLights Whether or not the shader uses multiple lights
    * @return whether or not initialisation succeeded
    */
    bool InitialiseFromFragments(const std::string& name, bool usesMultipleLights);

    /**
    * Sets the constants for the shader
    * @param services Interface providing methods for communicating with shaders
    * @param userData User defined int specific on shader creation
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

private:

    /**
    * Does custom pre-processing of the shader files before sending them to irrlicht
    * This allows shaders to be constructed from shader fragments
    * @param name The name of the shader
    * @param isVertex Whether or not this shader is the vertex shader
    * @param usesMultipleLights Whether or not this shader uses multiple lights
    * @return whether or not the generation failed
    */
    bool CreateShaderFromFragments(const std::string& name, bool isVertex, bool usesMultipleLights);

    /**
    * Reads from a particular file and adds lines to the new shader file
    * @param filepath The path of the file to open and read from
    * @param assetspath The assets path of all shaders
    * @param newfile The new file to add lines from the opened file
    * @return whether the call succeeded
    */
    bool ReadFromFile(const std::string& filepath, const std::string& assetspath, std::ofstream& newfile);

    /**
    * Adds the shader component to the list of components if the name allows it
    * @param component The component to add
    * @param name The name of the shader
    */
    void AddShaderComponent(const std::string& component,const std::string& name);

    /**
    * Adds the shader component to the list of components
    * @param component The component to add
    */
    void AddShaderComponent(const std::string& component);

    /**
    * Reads the file until a specific string is found
    * @param assetspath The assets path of all shaders
    * @param file The file to read
    * @param newfile The target string to read until
    * @param target The new file to add lines from the opened file
    * @Param skiplines Whether to write to the new file or not
    * @return the last line read after the target string is found
    */
    typedef std::vector<std::string> TargetVector;
    std::string ReadFile(const std::string& assetspath, std::ifstream& file, 
        std::ofstream& newfile, const TargetVector& target, bool skiplines);

    boost::shared_ptr<ShaderEditor> m_editor; ///< Editor for handling component visilibity
    std::vector<std::string> m_shaderComponent;  ///< Components apart of this shader
};