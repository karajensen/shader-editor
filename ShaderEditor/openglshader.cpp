////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglshader.h"
#include "boost/algorithm/string.hpp"
#include "boost/bimap.hpp"
#include "boost/assign.hpp"
#include <iomanip>
#include <assert.h>
#include <fstream>
#include "elements.h"

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

    // How many float components to the GLSL float structure
    boost::bimap<int, GLenum> FLOAT_COMPONENTS = 
        boost::assign::list_of<boost::bimap<int, GLenum>::relation>
        (1, GL_FLOAT)
        (2, GL_FLOAT_VEC2)
        (3, GL_FLOAT_VEC3)
        (4, GL_FLOAT_VEC4)
        (16, GL_FLOAT_MAT4);
}

GlShader::GlShader(const Shader& shader) :
    m_shader(shader),
    m_vsFilepath(shader.glslVertexFile),
    m_fsFilepath(shader.glslFragmentFile)
{
    m_vaFilepath = boost::ireplace_last_copy(
        m_vsFilepath, SHADER_EXTENSION, ASM_EXTENSION);
    
    m_faFilepath = boost::ireplace_last_copy(
        m_fsFilepath, SHADER_EXTENSION, ASM_EXTENSION);
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
    m_vertexAsm.clear();
    m_fragmentAsm.clear();

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
    std::string vertexText;
    std::string errorBuffer = LoadShaderText(m_vsFilepath, vertexText);
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

    GLint vertex = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexErrors = CompileShader(vertex, vertexText);
    if(!vertexErrors.empty())
    {
        vertexErrors = VS + vertexErrors;
    }

    GLint fragment = glCreateShader(GL_FRAGMENT_SHADER);
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

    Release();
    m_program = glCreateProgram();
    m_vs = vertex;
    m_fs = fragment;
    m_vertexText = vertexText;
    m_fragmentText = fragmentText;

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

    errorBuffer = LinkShaderProgram();
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = BindShaderAttributes();
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = FindShaderUniforms();
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    return std::string();
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
            std::string errors(errorLength, '\0');
            glGetShaderInfoLog(index, errorLength, 0, &errors[0]);
            return std::string(errors.begin(), errors.begin() + errors.find('\0'));
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
        std::string errors(bufferSize, '\0');
        glGetProgramInfoLog(m_program, bufferSize, 0, &errors[0]);
        return std::string(errors.begin(), errors.begin() + errors.find('\0'));
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

    // Shader Analyzer outputs file with multiple \0 at the end
    text = std::string(text.begin(), text.begin() + text.find('\0'));

    // Any error output from the analyzer will be in the generated file
    const int minimumLines = 3;
    return std::count(text.begin(), text.end(), '\n')
        <= minimumLines ? text : std::string();
}

std::string GlShader::GenerateAssembly()
{
    if(!m_vertexAsm.empty() && !m_fragmentAsm.empty())
    {
        return std::string();
    }

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

    std::string errorBuffer = LoadAssemblyText(m_vaFilepath, m_vertexAsm);
    if(!errorBuffer.empty())
    {
        return VS + "ShaderAnalyzer: " + errorBuffer;
    }

    errorBuffer = LoadAssemblyText(m_faFilepath, m_fragmentAsm);
    if(!errorBuffer.empty())
    {
        return FS + "ShaderAnalyzer: " + errorBuffer;
    }

    return std::string();
}

std::string GlShader::BindShaderAttributes()
{
    std::string errorBuffer = BindVertexAttributes();
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    glBindFragDataLocation(m_program, 0, GLSL_OUT_COLOR.c_str());
    if(HasCallFailed())
    {
        return FS + "Failed to bind " + GLSL_OUT_COLOR;
    }

    return errorBuffer;
}

std::string GlShader::BindVertexAttributes()
{
    // GLSL will optimise out any unused attributes which reflection
    // cannot obtain. Need to read the shader code to retrieve these
    // as the vertex stride of the buffers cannot be changed.

    m_stride = 0;
    int index = 0;
    int location = 0;

    // Split the shader text into words
    auto deliminator = boost::is_any_of(";\n\r\t ");
    std::vector<std::string> text;
    boost::split(text, m_vertexText, deliminator, boost::token_compress_on);

    // Ordering of input attributes is assumed to be: 'in type name'
    while(text[index] != MAIN_ENTRY)
    {
        if(text[index] == GLSL_IN)
        {
            AttributeData data;
            data.name = text[index+2];
            const std::string type = text[index+1];

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

            data.location = location++;
            m_stride += data.components;
            index += 2;

            m_attributes.push_back(data);
            glBindAttribLocation(m_program, data.location, data.name.c_str());
            if(HasCallFailed())
            {
                return "Failed to bind attribute " + data.name;
            }
        }
        else
        {
            ++index;
        }
    }

    return std::string();
}

std::string GlShader::FindShaderUniforms()
{
    int maxLength;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    int uniformCount;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);

    if(HasCallFailed())
    {
        return "Could not get uniform count for shader " + m_shader.name;
    }

    for (int i = 0; i < uniformCount; ++i)
    {
        int size;
        GLenum type;
        std::string name(maxLength,'\0');
        glGetActiveUniform(m_program, i, maxLength, 0, &size, &type, &name[0]);
        name = std::string(name.begin(), name.begin() + name.find('\0'));
        if(HasCallFailed())
        {
            return "Could not get uniform " + boost::lexical_cast<std::string>(i);
        }

        GLint location = glGetUniformLocation(m_program, name.c_str());
        if(HasCallFailed() || location == NO_INDEX)
        {
            return "Could not find uniform " + name + " for shader " + m_shader.name;
        }
        
        if(type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_MULTISAMPLE)
        {
            m_samplers.push_back(location);
        }
        else
        {
            m_uniforms[name].location = location;
            m_uniforms[name].type = type;
        }
    }

    return std::string();
}

void GlShader::SendUniformMatrix(const std::string& name, const glm::mat4& matrix)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end() && CanSendUniform(GL_FLOAT_MAT4, itr->second.type, name))
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
    const GLenum type = GetTypeFromComponents(size);

    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end() && CanSendUniform(type, itr->second.type, name))
    {
        switch(size)
        {
        case 1:
            glUniform1f(itr->second.location, value[0]);
            break;
        case 2:
            glUniform2f(itr->second.location, value[0], value[1]);
            break;
        case 3:
            glUniform3f(itr->second.location, value[0], value[1], value[2]);
            break;
        case 4:
            glUniform4f(itr->second.location, value[0], value[1], value[2], value[3]);
            break;
        default:
            Logger::LogError("Size too large for uniform " + name);
        }

        if(HasCallFailed())
        {
            Logger::LogError("Could not send uniform " + name);
        }
    }
}

GLenum GlShader::GetTypeFromComponents(int components) const
{
    auto itr = FLOAT_COMPONENTS.left.find(components);
    if(itr != FLOAT_COMPONENTS.left.end())
    {
        return itr->second;
    }
    Logger::LogError("Could not find type in list");
    return 0;
}

int GlShader::GetComponentsFromType(GLenum type) const
{
    auto itr = FLOAT_COMPONENTS.right.find(type);
    if(itr != FLOAT_COMPONENTS.right.end())
    {
        return itr->second;
    }
    Logger::LogError("Could not components in list");
    return 0;
}

bool GlShader::CanSendUniform(GLenum expectedType, 
                              GLenum actualType, 
                              const std::string& name) const
{
    if(actualType != expectedType)
    {
        Logger::LogError(name + " type mismatch. Attempting to send " + 
            boost::lexical_cast<std::string>(GetComponentsFromType(actualType)) + " as type " + 
            boost::lexical_cast<std::string>(GetComponentsFromType(expectedType)));
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

void GlShader::ClearTexture(int slot, bool multisample)
{
    glActiveTexture(GetTexture(slot));

    glBindTexture(!multisample ? GL_TEXTURE_2D :
        GL_TEXTURE_2D_MULTISAMPLE, 0);

    if(HasCallFailed())
    {
        Logger::LogError("Could not clear texture");
    }
}

void GlShader::SendTexture(int slot, GLuint id, bool multisample)
{
    glActiveTexture(GetTexture(slot));

    glBindTexture(!multisample ? GL_TEXTURE_2D :
        GL_TEXTURE_2D_MULTISAMPLE, id);

    glUniform1i(m_samplers[slot], slot);

    if(HasCallFailed())
    {
        Logger::LogError("Could not send texture");
    }
}

void GlShader::SetActive()
{
    glUseProgram(m_program);
}

int GlShader::GetIndex() const
{
    return m_shader.index;
}

bool GlShader::HasTextureSlot(int slot)
{
    return slot < static_cast<int>(m_samplers.size());
}

std::string GlShader::GetText() const
{
    return m_vertexText + "\n" + m_fragmentText;
}

std::string GlShader::GetAssembly()
{
    // Generating the assembly is a bottleneck for OpenGL
    // as requires ShaderAnalyzer. Only generate when requested.
    const std::string errors = GenerateAssembly();
    if(!errors.empty())
    {
        Logger::LogError("OpenGL: " + m_shader.name + " " + errors);
    }
    return m_vertexAsm + "\n" + m_fragmentAsm;
}

const std::string& GlShader::GetName() const
{
    return m_shader.name;
}