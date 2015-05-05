////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include "common.h"

class Shader;
class PostProcessing;

/**
* Generates a shader with the required components from a set of base shader fragments
*/
class FragmentLinker : boost::noncopyable
{
public:

    /**
    * Initialises the fragment linker
    * @param maxLights The amount of lights the shader will consider
    * @param post The post processing data
    * @return Whether initialisation was successful
    */
    bool Initialise(unsigned int maxLights, const PostProcessing& post);

    /**
    * Generates a shader from base shader fragments
    * @param shader The shader object to fill in
    * @return Whether generation was successful
    */
    bool GenerateWithFragments(Shader& shader);

    /**
    * Generates a shader from file
    * @param shader The shader object to fill in
    * @return Whether generation was successful
    */
    bool GenerateFromFile(Shader& shader);

private:

    /**
    * Initialises a shader from file
    * @param name The name of the shader
    * @param extension The extension for a hlsl or glsl shader
    * @return Whether generation was successful
    */
    bool GenerateFromFile(const std::string& name, const std::string& extension);

    /**
    * Creates the folder to hold all generated shaders
    * @return Whether creation was successful
    */
    bool CreateGeneratedFolder();

    /**
    * Generates a shader with the required components from the base shader
    * @param name The name of the shader to generate
    * @param extension The file extension of the shader
    * @return Whether generation was successful
    */
    bool CreateShaderFromFragments(const std::string& name, 
                                   const std::string& extension);

    /**
    * Reads the base shader until the end of the file
    * Any components the generated shader requires will be written to it
    * @note Used recursively when determining component conditionals
    * @param baseFile The filestream for the base shader
    * @param generatedFile The filestream of the shader being created
    * @param targets The target strings to read the base file until
    * @param previousLine The line last added to the generated shader
    * @param skiplines Whether to write to the generated file or not
    * @param level The recursive level this function is on
    * @return the last line read after the target string is found
    */
    std::string ReadBaseShader(std::ifstream& baseFile, 
                               std::ofstream& generatedFile, 
                               const std::vector<std::string>& targets, 
                               std::string& previousLine, 
                               bool skiplines, 
                               int level);

    /**
    * Determines if given line is a conditional and solves it if so
    * @param level The recursive level ReadBaseShader() is on
    * @param line The current line read from the base shader
    * @param previousLine The line last added to the generated shader
    * @param baseFile The filestream for the base shader
    * @param generatedFile The filestream of the shader being created
    * @param skiplines Whether to write to the generated file or not
    * @return whether the line was a conditional statement or not
    */
    bool SolveConditionalLine(int level, 
                              std::string line, 
                              std::string& previousLine,
                              std::ifstream& baseFile,
                              std::ofstream& generatedFile,
                              bool skiplines);
    /**
    * Gets the next line from the file, filtering and removing as needed
    * @param file The shader file to read from
    * @return the next line from the file
    */
    std::string GetNextLine(std::ifstream& file) const;

    /**
    * Determines whether the conditional if-else block should be included
    * @param conditional The conditional keyword of the block
    * @param line The line with the conditional keyword
    * @return whether the block should be included in the generated shader or not
    */
    bool ShouldSkipConditionalBlock(
        const std::string& conditional, std::string line) const;

    std::unordered_map<std::string, std::string> m_defines; ///< map of #defined items
    std::vector<std::string> m_shaderComponents; ///< components of currently linked shader
};

