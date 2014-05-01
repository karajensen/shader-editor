////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglshader.h"
#include "boost/algorithm/string.hpp"
#include <iomanip>
#include <assert.h>

namespace
{
    const std::string MAIN_ENTRY("main(void)"); ///< Text for the shader main entry point
    const std::string VS("Vertex Shader: ");    ///< Text for vertex shader diagnostics
    const std::string FS("Fragment Shader: ");  ///< Text for fragment shader diagnostics
}

GlShader::GlShader(int index, const std::string& vs, const std::string& fs) :
    m_vsFilepath(vs),
    m_fsFilepath(fs),
    m_program(NO_INDEX),
    m_vs(NO_INDEX),
    m_fs(NO_INDEX),
    m_index(index),
    m_stride(0)
{
}

GlShader::~GlShader()
{
    Release();
}

void GlShader::Release()
{
    m_uniforms.clear();
    m_attributes.clear();

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
        return VS + errorBuffer;
    }

    errorBuffer = LoadShaderFile(m_fsFilepath, fSize, fText);
    if(!errorBuffer.empty())
    {
        return FS + errorBuffer;
    }

    // Test on the scratch shaders to give an overview of any compilation errors
    std::string vertexErrors = CompileShader(scratchVS, vText.c_str(), vSize);
    if(!vertexErrors.empty())
    {
        vertexErrors = VS + vertexErrors;
    }

    std::string fragmentErrors = CompileShader(scratchFS, fText.c_str(), fSize);
    if(!fragmentErrors.empty())
    {
        fragmentErrors = FS + fragmentErrors;
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
        return VS + errorBuffer;
    }

    errorBuffer = CompileShader(m_fs, fText.c_str(), fSize);
    if(!errorBuffer.empty())
    {
        return FS + errorBuffer;
    }

    auto deliminator = boost::is_any_of(";\n\r ");
    std::vector<std::string> splitVertexText, splitFragmentText;
    boost::split(splitVertexText, vText, deliminator, boost::token_compress_on);
    boost::split(splitFragmentText, fText, deliminator, boost::token_compress_on);

    errorBuffer = BindVertexAttributes(splitVertexText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

	glAttachShader(m_program, m_vs);
    if(HasCallFailed())
    {
        return VS + "Failed to attach";
    }

    glAttachShader(m_program, m_fs);
    if(HasCallFailed())
    {
        return FS + "Failed to attach";
    }

    glBindFragDataLocation(m_program, 0, "outColor");
    if(HasCallFailed())
    {
        return FS + "Failed to bind outColour";
    }

    errorBuffer = LinkShaderProgram();
    if(!errorBuffer.empty())
    {
        return "Failed to link program: " + errorBuffer;
    }

    errorBuffer = FindShaderUniforms(splitVertexText);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = FindShaderUniforms(splitFragmentText);
    if(!errorBuffer.empty())
    {
        return FS + errorBuffer;
    }

    return std::string();
}

std::string GlShader::BindVertexAttributes(const std::vector<std::string>& vText)
{
    m_stride = 0;
    int currentIndex = 0;
    int currentLocation = 0;
    while(vText[currentIndex] != MAIN_ENTRY)
    {
        if(vText[currentIndex] == "in")
        {
            AttributeData data;
            data.name = vText[currentIndex+2];

            const std::string type = vText[currentIndex+1];
            if(type == "vec3")
            {
                data.components = 3;
            }
            else if(type == "vec2")
            {
                data.components = 2;
            }
            else if(type == "vec4")
            {
                data.components = 4;
            }
            else
            {
                return "Unknown attribute type for " + data.name;
            }

            m_stride += data.components;
            data.location = currentLocation;
            m_attributes.push_back(data);
            currentIndex += 2;
            ++currentLocation;
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
        glBindAttribLocation(m_program, attribute.location, attribute.name.c_str());
        if(HasCallFailed())
        {
            return "Failed to bind attribute " + attribute.name;
        }
    }
    return std::string();
}

std::string GlShader::FindShaderUniforms(const std::vector<std::string>& text)
{
    int currentIndex = 0;
    while(text[currentIndex] != MAIN_ENTRY)
    {
        if(text[currentIndex] == "uniform")
        {
            const std::string& name = text[currentIndex+2];
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if(HasCallFailed() || location == NO_INDEX)
            {
                return "Could not find uniform " + name;
            }
            m_uniforms[name].location = location;
            m_uniforms[name].type = text[currentIndex+1];
            currentIndex += 2;
        }
        else
        {
            ++currentIndex;
        }
    }
    return std::string();
}

void GlShader::SendUniformMatrix(const std::string& name, const glm::mat4& matrix)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end() && CanSendUniform("mat4", itr->second.type, name))
    {
        glUniformMatrix4fv(itr->second.location, 1, GL_FALSE, &matrix[0][0]);
        if(HasCallFailed())
        {
            Logger::LogError("Could not send uniform " + name);
        }
    }
}

void GlShader::SendUniformFloat(const std::string& name, float value)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end() && CanSendUniform("float", itr->second.type, name))
    {
        glUniform1f(itr->second.location, value);
        if(HasCallFailed())
        {
            Logger::LogError("Could not send uniform " + name);
        }
    }
}

bool GlShader::CanSendUniform(const std::string& expectedType, 
                              const std::string& actualType, 
                              const std::string& name) const
{
    if(actualType != expectedType)
    {
        Logger::LogError(name + " type mismatch. Attempting to send " + 
            actualType + " as a " + expectedType);
        return false;
    }
    return true;
}

void GlShader::EnableAttributes()
{
    int offset = 0;
    for(const AttributeData& attr : m_attributes)
    {
        glEnableVertexAttribArray(attr.location);
        if(HasCallFailed())
        {
            Logger::LogError("Could not enable attribute " + attr.name);
        }
    
        glVertexAttribPointer(attr.location, attr.components, GL_FLOAT, 
            GL_FALSE, m_stride*sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));
        if(HasCallFailed())
        {
            Logger::LogError("Could not set attribute " + attr.name);
        }
    
        offset += attr.components;
    }
}

void GlShader::SetAsActive()
{
    glUseProgram(m_program);
}

int GlShader::GetIndex() const
{
    return m_index;
}