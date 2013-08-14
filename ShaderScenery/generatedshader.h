////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - generatedshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <array>
#include <fstream>
#include "shader.h"

/**
* Fragment linker to auto-generate shaders for each mesh
*/
class GeneratedShader : public Shader
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit GeneratedShader(EnginePtr engine);

    /**
    * Destructor
    */
    ~GeneratedShader();

    /**
    * Shader Components avaliable for editing
    */
    enum ComponentVisibility
    {
        SPECULAR_VISIBILITY,
        BUMP_VISIBILITY,
        PARALLAX_VISIBILITY,
        SOFTSHADOW_VISIBILITY,
        MAX_EDITABLE
    };

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

    /**
    * @return a vector of shader component names
    */
    static std::vector<std::string> GetShaderComponents();

    /**
    * Sets the visiblity of the editable component
    * @param component The component to set
    * @param value The level of visibilty of the component
    */
    static void SetComponentVisibility(unsigned int component, float value);

    /**
    * @param component The component to get a description for
    * @return a string description of the editable component
    */
    static stringw GetComponentDescription(unsigned int component);

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

    std::vector<std::string> m_shaderComponent;  ///< Components part of this shader
    static std::array<float, MAX_EDITABLE> sm_editableComponents;  ///< which components are can be edited
};