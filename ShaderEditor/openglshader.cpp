////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglshader.h"
#include "boost/algorithm/string.hpp"
#include <iomanip>
#include <assert.h>

GlShader::GlShader(const std::string& vs, const std::string& fs) :
    m_vsFilepath(vs),
    m_fsFilepath(fs),
    m_program(NO_INDEX),
    m_vs(NO_INDEX),
    m_fs(NO_INDEX)
{
}

GlShader::~GlShader()
{
    Release();
}

void GlShader::Release()
{
    if(m_program != NO_INDEX)
    {
        if(m_vs != NO_INDEX)
        {
            glDetachShader(m_program, m_vs);
        }
        if(m_fs != NO_INDEX)
        {
            glDetachShader(m_program, m_fs);
        }
        glDeleteProgram(m_program);
        m_program = NO_INDEX;
    }
    if(m_vs != NO_INDEX)
    {
        glDeleteShader(m_vs);
        m_vs = NO_INDEX;
    }
    if(m_fs != NO_INDEX)
    {
        glDeleteShader(m_fs);
        m_fs = NO_INDEX;
    }
}

GLint GlShader::GetProgram() const
{
    return m_program;
}

std::string GlShader::BindShaderAttributes(const std::string& vsText)
{
    m_attributes.clear();
    std::vector<std::string> components;
    boost::split(components, vsText, boost::is_any_of(";\n\r "), boost::token_compress_on);

    int currentIndex = 0;
    int currentRegister = 0;
    while(components[currentIndex] != "main(void)")
    {
        if(components[currentIndex] == "in")
        {
            currentIndex += 2;
            m_attributes[components[currentIndex]] = currentRegister;
            ++currentRegister;
        }
        else
        {
            ++currentIndex;
        }
    }

    if(m_attributes.empty())
    {
        return "Failed to find any attributes";
    }

    for(const auto& attribute : m_attributes)
    {
        glBindAttribLocation(m_program, attribute.second, attribute.first.c_str());
        if(HasCallFailed())
        {
            return "Failed to bind attribute " + attribute.first;
        }
    }

    return std::string();
}

std::string GlShader::CompileShader(GLint index, const char* source, int size)
{
    GLint success = GL_FALSE;
    glShaderSource(index, 1, &source, &size);
    glCompileShader(index);
    glGetShaderiv(index, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        int errorLength = 0;
        glGetShaderiv(index, GL_INFO_LOG_LENGTH, &errorLength);
        if(errorLength <= 0)
        {
            return "Unknown Error";
        }
        else
        {
            int actualLength = 0;
            std::string errorBuffer(errorLength, ' ');
            glGetShaderInfoLog(index, errorLength, &actualLength, &errorBuffer[0]);
            errorBuffer.resize(actualLength);
            return errorBuffer;
        }
    }
    return std::string();
}

std::string GlShader::LoadShaderFile(const std::string& path, int& size, std::string& text)
{
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
    if(file.is_open())
    {
        size = static_cast<int>(file.tellg());
        file.seekg(0, std::ios::beg);
        text.resize(size);
        file.read(&text[0], text.size());
        assert(!text.empty());
        file.close();
    }
    else
    {
       return "Could not open file " + path;
    }
    return std::string();
}

std::string GlShader::LinkShaderProgram()
{
    GLint linkSuccess = GL_FALSE;
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE)
    {
        const int bufferSize = 1024;
        std::string errorBuffer(bufferSize, ' ');
        glGetProgramInfoLog(m_program, bufferSize, 0, &errorBuffer[0]);
        return errorBuffer;
    }
    return std::string();
}

std::string GlShader::CompileShader(GLint scratchVS, GLint scratchFS)
{
    std::string errorBuffer;
    int vSize = 0, fSize = 0;
    std::string vText, fText;

    // Load the vertex and fragment shader files
    errorBuffer = LoadShaderFile(m_vsFilepath, vSize, vText);
    if(!errorBuffer.empty())
    {
        return "Vertex Shader: " + errorBuffer;
    }

    errorBuffer = LoadShaderFile(m_fsFilepath, fSize, fText);
    if(!errorBuffer.empty())
    {
        return "Fragment Shader: " + errorBuffer;
    }

    // Test on the scratch shaders to give an overview of any compilation errors
    std::string vertexErrors = CompileShader(scratchVS, vText.c_str(), vSize);
    if(!vertexErrors.empty())
    {
        vertexErrors = "Vertex Shader: " + vertexErrors;
    }

    std::string fragmentErrors = CompileShader(scratchFS, fText.c_str(), fSize);
    if(!fragmentErrors.empty())
    {
        fragmentErrors = "Fragment Shader: " + fragmentErrors;
    }

    if(!fragmentErrors.empty() || !vertexErrors.empty())
    {
        return vertexErrors.empty() ? fragmentErrors :
            vertexErrors + (fragmentErrors.empty() ? "" : "\n"+fragmentErrors);
    }

    // Recreate the actual shader/program
    Release();
    m_program = glCreateProgram();
    m_vs = glCreateShader(GL_VERTEX_SHADER);
    m_fs = glCreateShader(GL_FRAGMENT_SHADER);

    errorBuffer = CompileShader(m_vs, vText.c_str(), vSize);
    if(!errorBuffer.empty())
    {
        return "Vertex Shader " + errorBuffer;
    }

    errorBuffer = CompileShader(m_fs, fText.c_str(), fSize);
    if(!errorBuffer.empty())
    {
        return "Fragment Shader " + errorBuffer;
    }

    errorBuffer = BindShaderAttributes(vText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

	glAttachShader(m_program, m_vs);
    if(HasCallFailed())
    {
        return "Failed to attach vertex shader";
    }

    glAttachShader(m_program, m_fs);
    if(HasCallFailed())
    {
        return "Failed to attach fragment shader";
    }

    errorBuffer = LinkShaderProgram();
    if(!errorBuffer.empty())
    {
        return "Failed to link program: " + errorBuffer;
    }

    return std::string();
}
