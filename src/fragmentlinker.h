////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <unordered_map>
#include <boost/noncopyable.hpp>

class Shader;

/**
* Generates a shader from a file replacing any special syntax or defined values
*/
class FragmentLinker : boost::noncopyable
{
public:

    FragmentLinker() = default;
    ~FragmentLinker() = default;

    /**
    * Initialises the fragment linker
    * @param maxWaves The amount of overlapping waves for water
    * @param maxLights The amount of lights the shader will consider
    * @param blurWeights The values for blurring surrounding pixels
    * @return Whether initialisation was successful
    */
    bool Initialise(unsigned int maxWaves,
                    unsigned int maxLights, 
                    const std::vector<float>& blurWeights);

    /**
    * Generates a new shader
    * @param shader The shader information to generate from
    * @return Whether generation was successful
    */
    bool GenerateShader(const Shader& shader);

private:

    /**
    * Generates a new shader
    * @param baseFilePath path of the file to use as a base for generation
    * @param generatedFilePath path of the file to save to once generated
    * @param generateFromFragments Whether to create a new shader based off conditionals
    * @return Whether generation was successful
    */
    bool GenerateShader(const std::string& baseFilePath, 
                        const std::string& generatedFilePath,
                        bool generateFromFragments);

    /**
    * Reads the base shader until the end of the file
    * @param baseFile The filestream for the base shader
    * @param generatedFile The filestream of the shader being created
    * @param previousLine The line last added to the generated shader
    * @return the last line read after the target string is found
    */
    std::string ReadBaseShader(std::ifstream& baseFile, 
                               std::ofstream& generatedFile, 
                               std::string& previousLine);

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
    * Gets the next line from the file, filtering and removing syntax as needed
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
    bool ShouldSkipConditionalBlock(const std::string& conditional, 
                                    std::string line) const;

private:

    std::unordered_map<std::string, std::string> m_defines; ///< map of #defined items to replace
    unsigned int m_shaderComponents;                        ///< components of shader undergoing linking
};
