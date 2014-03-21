////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <array>
#include <boost/noncopyable.hpp>

struct Shader;

/**
* Links part of a shader together to output a filename of the generated shader
* Also holds visibility modifiers for each shared component of linked shaders
*/
class FragmentLinker : boost::noncopyable
{
public:


    /**
    * Constructor
    */
    FragmentLinker();

    /**
    * Destructor
    */
    ~FragmentLinker();

    /**
    * Initialises the shader from fragments
    * @param shader The shader object to fill in 
    * @param maxLights The amount of lighting the shader will consider
    * @return whether or not initialisation succeeded
    */
    bool InitialiseFromFragments(Shader& shader, unsigned int maxLights);

private:

    /**
    * Adds the shader component to the list of components if the name allows it
    * @param component The component to add
    * @param name The name of the shader
    */
    void AddShaderComponent(const std::string& component, const std::string& name);

    /**
    * Finds the relevant components required to generate the shader
    * @param shader The shader object
    */
    void FindShaderComponents(Shader& shader);

    /**
    * Does custom pre-processing of the shader files before sending them to irrlicht
    * This allows shaders to be constructed from shader fragments
    * @param name The name of the shader
    * @param extension The file extension of the shader
    * @param maxLights The amount of lighting the shader will consider
    * @return whether or not the generation failed
    */
    bool CreateShaderFromFragments(const std::string& name, 
        const std::string& extension,
        boost::optional<unsigned int> maxLights);

    /**
    * Reads the base shader until the end of the file
    * Any components the generated shader requires will be copied to it.
    * @param baseFile The file for the base shader
    * @param generatedFile The shader being created
    * @param targets The target strings to read the base file until
    * @param skiplines Whether to write to the generated file or not
    * @param level The recursive level ReadBaseShader is on
    * @return the last line read after the target string is found
    */
    std::string ReadBaseShader(std::ifstream& baseFile, std::ofstream& generatedFile, 
        const std::vector<std::string>& targets, bool skiplines, int level);

    /**
    * Determines if given line is a conditional and solves it if so
    * @param line The given line to solve
    * @param baseFile The file for the base shader
    * @param generatedFile The shader being created
    * @param skiplines Whether to write to the generated file or not
    * @param level The recursive level ReadBaseShader is on
    * @return whether the line was a conditional statement or not
    */
    bool SolveConditionalLine(int level, std::string line, 
        std::ifstream& baseFile, std::ofstream& generatedFile, bool skiplines);

    std::vector<std::string> m_shaderComponents;  ///< components of the shader currently being linked
};

