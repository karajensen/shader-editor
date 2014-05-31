////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglshader.h"
#include "boost/algorithm/string.hpp"
#include <iomanip>
#include <assert.h>
#include <fstream>

namespace
{
    bool SHOW_CONSOLE_WINDOW = false; ///< Whether to show the ShaderAnalyzer window
    const std::string VS("Vertex Shader: "); ///< Text for vertex shader diagnostics
    const std::string FS("Fragment Shader: "); ///< Text for fragment shader diagnostics

    // GLSL Keywords
    const std::string ENTRY_NAME("main");
    const std::string MAIN_ENTRY(ENTRY_NAME + "(void)");
    const std::string VERTEX_MODEL("glsl_vs");
    const std::string FRAGMENT_MODEL("glsl_fs");
    const std::string GLSL_FLT("float");
    const std::string GLSL_VEC_PREFIX("vec");
    const std::string GLSL_VEC2(GLSL_VEC_PREFIX + "2");
    const std::string GLSL_VEC3(GLSL_VEC_PREFIX + "3");
    const std::string GLSL_VEC4(GLSL_VEC_PREFIX + "4");
    const std::string GLSL_MAT4("mat4");
    const std::string GLSL_IN_POSITION("in_Position");
    const std::string GLSL_OUT_COLOR("outColor");
    const std::string GLSL_IN("in");
    const std::string GLSL_UNIFORM("uniform");
    const std::string GLSL_SAMPLER2D("sampler2D");
}

GlShader::GlShader(int index, 
                   const std::string& vs, 
                   const std::string& fs) :

    m_vsFilepath(vs),
    m_fsFilepath(fs),
    m_program(NO_INDEX),
    m_vs(NO_INDEX),
    m_fs(NO_INDEX),
    m_index(index),
    m_stride(0)
{
    m_vaFilepath = boost::ireplace_last_copy(
        m_vsFilepath, SHADER_EXTENSION, ASM_EXTENSION);
    
    m_faFilepath = boost::ireplace_last_copy(
        m_fsFilepath, SHADER_EXTENSION, ASM_EXTENSION);
}

GlShader::GlShader(const std::string& vs, 
                   const std::string& fs,
                   const std::string& vsAsm,
                   const std::string& fsAsm) :

    m_vsFilepath(vs),
    m_fsFilepath(fs),
    m_vaFilepath(vsAsm),
    m_faFilepath(fsAsm),
    m_program(NO_INDEX),
    m_vs(NO_INDEX),
    m_fs(NO_INDEX),
    m_index(NO_INDEX),
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
    m_samplers.clear();

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

std::string GlShader::CompileShader()
{
    std::string errorBuffer;

    // Load the vertex and fragment shader files
    std::string vertexText;
    errorBuffer = LoadShaderText(m_vsFilepath, vertexText);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    std::string fragmentText;
    errorBuffer = LoadShaderText(m_fsFilepath, fragmentText);
    if(!errorBuffer.empty())
    {
        return FS + errorBuffer;
    }

    // Test on the scratch shaders to give an overview of any compilation errors
    GLint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexErrors = CompileShader(vertex, vertexText);
    if(!vertexErrors.empty())
    {
        vertexErrors = VS + vertexErrors;
    }

    std::string fragmentErrors = CompileShader(fragment, fragmentText);
    if(!fragmentErrors.empty())
    {
        fragmentErrors = FS + fragmentErrors;
    }

    if(!fragmentErrors.empty() || !vertexErrors.empty())
    {
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return vertexErrors.empty() ? fragmentErrors :
            vertexErrors + (fragmentErrors.empty() ? "" : "\n"+fragmentErrors);
    }

    // Only update the shader once compilation errors have been checked
    Release();
    m_program = glCreateProgram();
    m_vs = vertex;
    m_fs = fragment;

    // Split the shader text into wordss
    auto deliminator = boost::is_any_of(";\n\r ");
    std::vector<std::string> vertexWords, fragmentWords;
    boost::split(vertexWords, vertexText, deliminator, boost::token_compress_on);
    boost::split(fragmentWords, fragmentText, deliminator, boost::token_compress_on);

    errorBuffer = BindVertexAttributes(vertexWords);
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

    glBindFragDataLocation(m_program, 0, GLSL_OUT_COLOR.c_str());
    if(HasCallFailed())
    {
        return FS + "Failed to bind " + GLSL_OUT_COLOR;
    }

    errorBuffer = LinkShaderProgram();
    if(!errorBuffer.empty())
    {
        return "Failed to link program: " + errorBuffer;
    }

    errorBuffer = FindShaderUniforms(vertexWords);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = FindShaderUniforms(fragmentWords);
    if(!errorBuffer.empty())
    {
        return FS + errorBuffer;
    }

    std::string vertexAsm, fragmentAsm;
    errorBuffer = GenerateAssembly(vertexAsm, fragmentAsm);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    UpdateShaderText(vertexText, fragmentText, vertexAsm, fragmentAsm);
    return std::string();
}

void GlShader::UpdateShaderText(const std::string& vText, 
                                const std::string& fText,
                                const std::string& vAsm, 
                                const std::string& fAsm)
{
    m_vertexText = vText;
    m_vertexAsm = vAsm;
    m_fragmentText = fText;
    m_fragmentAsm = fAsm;
}

std::string GlShader::CompileShader(GLint index, const std::string& text)
{
    const char* source = text.c_str();
    const int size = static_cast<int>(text.size());

    GLint success = GL_FALSE;
    glShaderSource(index, 1, &source, &size);
    glCompileShader(index);
    glGetShaderiv(index, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        // Get the compilation error message
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

std::string GlShader::LoadShaderText(const std::string& path, std::string& text)
{
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
    if(file.is_open())
    {
        // Read all from file directly into a string
        const int size = static_cast<int>(file.tellg());
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

std::string GlShader::LoadAssemblyText(const std::string& path, std::string& text)
{
    std::ifstream file(path.c_str(), std::ios::in|std::ios::ate|std::ios::_Nocreate);
    if(!file.is_open())
    {
        return "Could not open file " + path;
    }
    
    // Read all from file directly into a string
    const int size = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);   
    text.clear();
    text.resize(size);
    file.read(&text[0], text.size());
    if(text.empty())
    {
        return "Generated assembly file is empty";
    }
    
    // Any error output from the analyzer will be in the generated file
    const int linesForError = 3;
    if(std::count(text.begin(), text.end(), '\n') <= linesForError)
    {
        return text;
    }
    return std::string();
}

std::string GlShader::GenerateAssembly(std::string& vertexAsm, std::string& fragmentAsm)
{
    // Opengl 3.0 currently has no output of shader asm 
    // Use cmd.exe to chain both calls to ShaderAnalyzer in one process
    // GPUShaderAnalyzer.exe Diffuse_glsl_vert.fx -I Diffuse_glsl_vert.asm -ASIC IL -profile glsl_vs -function main
    // GPUShaderAnalyzer.exe Diffuse_glsl_frag.fx -I Diffuse_glsl_frag.asm -ASIC IL -profile glsl_fs -function main

    const std::string process("Assets\\ShaderAnalyzer\\GPUShaderAnalyzer.exe ");
    const std::string console("C:\\windows\\system32\\cmd.exe");

    const std::string vertex(m_vsFilepath + " -I " + m_vaFilepath 
        + " -ASIC IL -profile " + VERTEX_MODEL + " -function " + ENTRY_NAME);

    const std::string fragment(m_fsFilepath + " -I " + m_faFilepath 
        + " -ASIC IL -profile " + FRAGMENT_MODEL + " -function " + ENTRY_NAME);

    const std::string command = "/C " + process + vertex + " && " + process + fragment + 
        (SHOW_CONSOLE_WINDOW ? " && pause" : "");

    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.cb = sizeof(si);
    if(!SHOW_CONSOLE_WINDOW)
    {
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
    }

    if(CreateProcess(console.c_str(), LPSTR(command.c_str()), 0, 0, FALSE, 
        (SHOW_CONSOLE_WINDOW ? 0 : CREATE_NO_WINDOW), 0, 0, &si, &pi))
    {
        ::WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        return "Process failed: " + command;
    }

    std::string errorBuffer = LoadAssemblyText(m_vaFilepath, vertexAsm);
    if(!errorBuffer.empty())
    {
        return VS + "ShaderAnalyzer: " + errorBuffer;
    }

    errorBuffer = LoadAssemblyText(m_faFilepath, fragmentAsm);
    if(!errorBuffer.empty())
    {
        return FS + "ShaderAnalyzer: " + errorBuffer;
    }

    return std::string();
}

std::string GlShader::BindVertexAttributes(const std::vector<std::string>& text)
{
    m_stride = 0;
    int currentIndex = 0;
    int currentLocation = 0;

    // Ordering of input attributes is assumed to be: 'in type name'
    while(text[currentIndex] != MAIN_ENTRY)
    {
        if(text[currentIndex] == GLSL_IN)
        {
            AttributeData data;
            data.name = text[currentIndex+2];
            const std::string type = text[currentIndex+1];

            // Pass position as a vec3 into a vec4 slot to use the optimization 
            // where the 'w' component is automatically set as 1.0
            if(type == GLSL_VEC3 || data.name == GLSL_IN_POSITION)
            {
                data.components = 3;
            }
            else if(type == GLSL_VEC2)
            {
                data.components = 2;
            }
            else if(type == GLSL_VEC4)
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
    // Find the vertex shader uniforms
    int currentIndex = 0;
    while(text[currentIndex] != MAIN_ENTRY)
    {
        if(text[currentIndex] == GLSL_UNIFORM)
        {
            const std::string& name = text[currentIndex+2];
            const std::string& type = text[currentIndex+1];
            GLint location = glGetUniformLocation(m_program, name.c_str());

            if(HasCallFailed() || location == NO_INDEX)
            {
                return "Could not find uniform " + name;
            }

            if(type == GLSL_SAMPLER2D)
            {
                m_samplers.push_back(location);
            }
            else
            {
                m_uniforms[name].location = location;
                m_uniforms[name].type = type;
            }

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
    if(itr != m_uniforms.end() && CanSendUniform(GLSL_MAT4, itr->second.type, name))
    {
        glUniformMatrix4fv(itr->second.location, 1, GL_FALSE, &matrix[0][0]);
        if(HasCallFailed())
        {
            Logger::LogError("Could not send uniform " + name);
        }
    }
}

void GlShader::SendUniformFloat(const std::string& name, const float* value, int size)
{
    std::string floatType;
    if(size == 1)
    {
        floatType = GLSL_FLT;
    }
    else
    {
        floatType = GLSL_VEC_PREFIX + boost::lexical_cast<std::string>(size);
    }
    
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end() && CanSendUniform(floatType, itr->second.type, name))
    {
        if(size == 1)
        {
            glUniform1f(itr->second.location, value[0]);
        }
        else if(size == 2)
        {
            glUniform2f(itr->second.location, value[0], value[1]);
        }
        else if(size == 3)
        {
            glUniform3f(itr->second.location, value[0], value[1], value[2]);
        }
        else if(size == 4)
        {
            glUniform4f(itr->second.location, value[0], value[1], value[2], value[3]);
        }
        else
        {
            Logger::LogError("Size too large for uniform " + name);
        }

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

void GlShader::SetActive()
{
    glUseProgram(m_program);

    // Enable the texture samplers once per activation
    int slot = 0;
    for(int location : m_samplers)
    {
        glUniform1i(location, slot++);
    }
}

int GlShader::GetIndex() const
{
    return m_index;
}

bool GlShader::HasTextureSlot(int slot)
{
    return slot < static_cast<int>(m_samplers.size());
}