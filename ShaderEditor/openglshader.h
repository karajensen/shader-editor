////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* Holds information for an individual opengl shader
*/
struct GlShader
{
public:

    /**
    * Constructor
    * @param vs The filepath for the vertex shader
    * @param fs The filepath for the fragment shader
    */
    GlShader(const std::string& vs, const std::string& fs);

    /**
    * Destructor
    */
    ~GlShader();

    /**
    * Releases the shader object
    */
    void Release();

    /**
    * Generates the shader for the engine
    * @param scratchVS A vertex shader to test changes on
    * @param scratchFS A fragment shader to test changes on
    * @return error message if failed or empty if succeeded
    */
    std::string CompileShader(GLint scratchVS, GLint scratchFS);

    /**
    * @return the program for the shader
    */
    GLint GetProgram() const;

private:

    /**
    * Determines the vertex shader 'in' attributes and caches them
    * @param vsText The text for the vertex shader
    * @return error message if failed or empty if succeeded
    */
    std::string BindShaderAttributes(const std::string& vsText);

    /**
    * Generates the shader for the engine
    * @param index The unique index of the shader (vs or fs) to compile
    * @param source The text to compile
    * @param size The amount of text to compile
    * @return error message if failed or empty if succeeded
    */
    std::string CompileShader(GLint index, const char* source, int size);

    /**
    * Loads the shader from the path
    * @param path The absolute path to the shader file
    * @param size The size of the file
    * @param text The text within the file
    * @return error message if failed or empty if succeeded
    */
    std::string LoadShaderFile(const std::string& path, int& size, std::string& text);

    /**
    * Links together all shaders within the shader program
    * @return error message if failed or empty if succeeded
    */
    std::string LinkShaderProgram();

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int location;        ///< The index location of the attribute
        std::string name;    ///< The name of the attribute
    };

    std::vector<AttributeData> m_attributes;  ///< Vertex shader attributes
    std::string m_vsFilepath;                 ///< Path to the vertex shader file
    std::string m_fsFilepath;                 ///< Path to the fragment shader file
	GLint m_program;                          ///< Shader program
	GLint m_vs;                               ///< GLSL Vertex Shader
	GLint m_fs;                               ///< GLSL Fragment Shader    
};