////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - generatedshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shader.h"
#include <fstream>

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
    * Adds the shader component to the list of components if the name allows it
    * @param component The component to add
    * @param name The name of the shader
    */
    void AddShaderComponent(const std::string& component, const std::string& name);

    /**
    * Does custom pre-processing of the shader files before sending them to irrlicht
    * This allows shaders to be constructed from shader fragments
    * @param name The name of the shader
    * @param isVertex Whether or not this shader is the vertex shader
    * @param usesMultipleLights Whether or not this shader uses multiple lights
    * @return whether or not the generation failed
    */
    bool CreateShaderFromFragments(const std::string& name, 
        bool isVertex, bool usesMultipleLights);

    /**
    * Reads the base shader until the end of the file
    * Any components the generated shader requires will be copied to it.
    * @param baseFile The file for the base shader
    * @param generatedFile The shader being created
    * @param targets The target strings to read the base file until
    * @param skiplines Whether to write to the generated file or not
    * @return the last line read after the target string is found
    */
    std::string ReadBaseShader(std::ifstream& baseFile, std::ofstream& generatedFile, 
        const std::vector<std::string>& targets, bool skiplines);

    /**
    * Determines if given line is a conditional and solves it if so
    * @param line The given line to solve
    * @param baseFile The file for the base shader
    * @param generatedFile The shader being created
    * @param skiplines Whether to write to the generated file or not
    * @return whether the line was a conditional statement or not
    */
    bool SolveConditionalLine(std::string line, std::ifstream& baseFile, 
        std::ofstream& generatedFile, bool skiplines);

    bool GeneratedShader::IsComponentDefined(std::string component);
        
    boost::shared_ptr<ShaderEditor> m_editor; ///< Editor for handling component visilibity
    std::vector<std::string> m_shaderComponent;  ///< Components apart of this shader
};