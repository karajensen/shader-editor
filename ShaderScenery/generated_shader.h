/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Shaders generated from fragments
*****************************************************************/
#pragma once

#include <array>
#include <fstream>
#include "shader.h"

class GeneratedShader : public Shader
{
public:

    ~GeneratedShader();
    GeneratedShader();

    /**
    * Shader Components avaliable for editing
    */
    enum ComponentVisibility
    {
        SPECULAR_VIS,
        BUMP_VIS,
        PARALLAX_VIS,
        SOFTSHADOW_VIS,
        MAX_EDITABLE
    };

    /**
    * Initialises the shader from fragments
    * @param the name of the shader
    * @param whether or not the shader uses multiple lights
    * @return whether or not initialisation succeeded
    */
    bool InitialiseFromFragments(const std::string& name, bool usesMultipleLights);

    /**
    * Sets the constants for the shader for a particular mesh
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * Retrieves a vector of shader component names
    */
    static std::vector<std::string> GetShaderComponents();

    /**
    * Sets the visiblity of the editable component
    * @param the component
    * @param the level of visibilty of the component
    */
    static void SetComponentVisibility(unsigned int component, float value);

    /**
    * @return a string description of the editable component
    */
    static stringw GetComponentDescription(unsigned int component);

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
    std::string ReadFile(const std::string& assetspath, std::ifstream& file, 
        std::ofstream& newfile, const TargetVector& target, bool skiplines);

    std::vector<std::string> m_shaderComponent;  ///< Components part of this shader
    static std::array<float, MAX_EDITABLE> sm_editableComponents;  ///< which components are currently toggled on/off
};