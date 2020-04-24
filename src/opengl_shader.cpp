////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_shader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl_shader.h"
#include "opengl_target.h"

#include "boost/algorithm/string.hpp"
#include "boost/bimap.hpp"
#include "boost/lexical_cast.hpp"

#include <iomanip>
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
    const std::string GLSL_HEADER("#version");

    const int IN_POSITION_ID = 0;
    std::vector<std::string> ATTRIBUTE_MAP =
    {
        "in_Position",
        "in_UVs",
        "in_Normal",
        "in_Tangent",
        "in_Bitangent",
    };
}

GlShader::GlShader(const Shader& shader)
    : m_shader(shader)
    , m_vsFilepath(shader.GLSLVertexFile())
    , m_fsFilepath(shader.GLSLFragmentFile())
    , m_vaFilepath(shader.GLSLVertexAsmFile())
    , m_faFilepath(shader.GLSLFragmentAsmFile())
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

    const std::string process("Assets\\Compiliers\\GPUShaderAnalyzer.exe ");
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

    glBindFragDataLocation(m_program, 0, "");
    if(HasCallFailed())
    {
        return FS + "Failed to bind fragment location";
    }

    return errorBuffer;
}

std::string GlShader::BindVertexAttributes()
{
    int maxLength;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    int attributeCount;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributeCount);

    if (HasCallFailed())
    {
        return "Could not get attribute count";
    }

    m_stride = 0;
    for (int i = 0; i < attributeCount; ++i)
    {
        int size;
        GLenum type;
        std::string name(maxLength, '\0');

        // Note attributes can be given in any order
        glGetActiveAttrib(m_program, i, maxLength, 0, &size, &type, &name[0]);
        name = std::string(name.begin(), name.begin() + name.find('\0'));

        if (HasCallFailed())
        {
            return "Could not get attribute " + std::to_string(i);
        }

        int location = NO_INDEX;
        for (int j = 0; j < static_cast<int>(ATTRIBUTE_MAP.size()); ++j)
        {
            if (ATTRIBUTE_MAP[j] == name)
            {
                location = j;
                break;
            }
        }

        if (location == NO_INDEX)
        {
            return "Unknown attribute name " + name;
        }

        m_attributes.emplace_back();
        m_attributes[i].location = location;
        m_attributes[i].name = name;

        // Pass position as a vec3 into a vec4 slot to use the optimization
        // where the 'w' component is automatically set as 1.0
        const bool isPosition = boost::iequals(m_attributes[i].name, ATTRIBUTE_MAP[IN_POSITION_ID]);
        m_attributes[i].components = GetComponents(isPosition ? GL_FLOAT_VEC3 : type);

        m_stride += m_attributes[i].components;
    }

    std::sort(m_attributes.begin(), m_attributes.end(), [](const AttributeData& d1, const AttributeData& d2)
    {
        return d1.location < d2.location;
    });

    for (const AttributeData& attr : m_attributes)
    {
        glBindAttribLocation(m_program, attr.location, attr.name.c_str());

        if (HasCallFailed())
        {
            return "Failed to bind attribute " + attr.name;
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
        return "Could not get uniform count for shader " + m_shader.Name();
    }

    int samplerSlot = 0;
    for (int i = 0; i < uniformCount; ++i)
    {
        int size;
        GLenum type;
        std::string name(maxLength,'\0');
        glGetActiveUniform(m_program, i, maxLength, 0, &size, &type, &name[0]);

        name = std::string(name.begin(), name.begin() + name.find('\0'));
        const int index = name.find("[");
        if (index != NO_INDEX)
        {
            name = std::string(name.begin(), name.begin() + index);
        }

        if(HasCallFailed())
        {
            return "Could not get uniform " + boost::lexical_cast<std::string>(i);
        }

        GLint location = glGetUniformLocation(m_program, name.c_str());
        if(HasCallFailed() || location == NO_INDEX)
        {
            return "Could not find uniform " + name + " for shader " + m_shader.Name();
        }
        
        if(type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_MULTISAMPLE || type == GL_SAMPLER_CUBE)
        {
            m_samplers[name].location = location;
            m_samplers[name].type = type;
            m_samplers[name].slot = samplerSlot;
            ++samplerSlot;
        }
        else
        {   
            // Scratch buffer not allocated for matrices
            const bool isMatrix = type == GL_FLOAT_MAT4;
            const int scratchBufferSize = isMatrix ? 0 : GetComponents(type) * size;

            m_uniforms[name].scratch.resize(scratchBufferSize);
            m_uniforms[name].scratch.assign(scratchBufferSize, 0.0f);
            m_uniforms[name].location = location;
            m_uniforms[name].type = type;
            m_uniforms[name].size = size;
        }
    }

    return std::string();
}

void GlShader::SendUniformMatrix(const std::string& name, const glm::mat4& matrix)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end())
    {
        glUniformMatrix4fv(itr->second.location, 1, GL_FALSE, &matrix[0][0]);

        if (itr->second.type != GL_FLOAT_MAT4)
        {
            Logger::LogError("Uniform " + name + " isn't a matrix");
        }

        if(HasCallFailed())
        {
            Logger::LogError("Could not send uniform " + name);
        }
    }
}

void GlShader::UpdateUniformArray(const std::string& name, const float* value, int count, int offset)
{
    auto itr = m_uniforms.find(name);
    if (itr != m_uniforms.end())
    {
        for(int i = offset, j = 0; j < count; ++i, ++j)
        {
            itr->second.scratch[i] = value[j];
        }

        itr->second.updated = true;
    }
}

void GlShader::SendUniformArrays()
{
    for (auto& uniform : m_uniforms)
    {
        if (uniform.second.updated)
        {
            uniform.second.updated = false;
            SendUniformFloat(
                uniform.first, 
                &uniform.second.scratch[0],
                uniform.second.location,
                uniform.second.size,
                uniform.second.type);
        }
    }
}

void GlShader::SendUniformFloat(const std::string& name, 
                                const float* value, 
                                int location, 
                                int size, 
                                GLenum type)
{
    switch(type)
    {
    case GL_FLOAT:
        glUniform1fv(location, size, value);
        break;
    case GL_FLOAT_VEC2:
        glUniform2fv(location, size, value);
        break;
    case GL_FLOAT_VEC3:
        glUniform3fv(location, size, value);
        break;
    case GL_FLOAT_VEC4:
        glUniform4fv(location, size, value);
        break;
    default:
        Logger::LogError("Unknown uniform type " + name);
    }

    if(HasCallFailed())
    {
        Logger::LogError("Could not send uniform " + name);
    }
}

void GlShader::SendUniformFloat(const std::string& name, const float* value, int count)
{
    auto itr = m_uniforms.find(name);
    if(itr != m_uniforms.end())
    {
        SendUniformFloat(name, value, itr->second.location, 
            itr->second.size, itr->second.type);
    }
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

void GlShader::ClearTexture(const std::string& sampler, const GlRenderTarget& target)
{
    ClearTexture(sampler, target.IsMultisampled(), false);
}

void GlShader::ClearTexture(const std::string& sampler, bool multisample, bool cubemap)
{
    auto samplerItr = m_samplers.find(sampler);
    if (samplerItr != m_samplers.end())
    {
        glActiveTexture(GetTexture(samplerItr->second.slot));

        if (cubemap)
        {
            glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
        }
        else
        {
            glBindTexture(!multisample ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, 0);
        }

        if(HasCallFailed())
        {
            Logger::LogError("Could not clear texture");
        }
    }
}

void GlShader::SendTexture(const std::string& sampler, GLuint id, bool cubemap)
{
    SendTexture(sampler, id, false, cubemap);
}

void GlShader::SendTexture(const std::string& sampler, GLuint id, bool multisample, bool cubemap)
{
    auto samplerItr = m_samplers.find(sampler);
    if (samplerItr != m_samplers.end())
    {
        if (samplerItr->second.allocated != id)
        {
            samplerItr->second.allocated = id;

            glActiveTexture(GetTexture(samplerItr->second.slot));

            if (cubemap)
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, id);
            }
            else
            {
                glBindTexture(!multisample ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, id);
            }

            glUniform1i(samplerItr->second.location, samplerItr->second.slot);

            if (HasCallFailed())
            {
                Logger::LogError("Could not send texture");
            }
        }
    }
}

void GlShader::SendTexture(const std::string& sampler, const GlRenderTarget& target, int ID)
{
    SendTexture(sampler, target.GetTexture(ID), target.IsMultisampled(), false);
}

void GlShader::SetActive()
{
    glUseProgram(m_program);
    for (auto& sampler : m_samplers)
    {
        sampler.second.allocated = NO_INDEX;
    }
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
        Logger::LogError("OpenGL: " + m_shader.Name() + " " + errors);
    }
    return m_vertexAsm + "\n" + m_fragmentAsm;
}

const std::string& GlShader::GetName() const
{
    return m_shader.Name();
}

std::string GlShader::GetShaderHeader()
{
    return GLSL_HEADER;
}

unsigned int GlShader::GetTexture(int slot)
{
    switch(slot)
    {
    case 0:
        return GL_TEXTURE0;
    case 1:
        return GL_TEXTURE1;
    case 2:
        return GL_TEXTURE2;
    case 3:
        return GL_TEXTURE3;
    case 4:
        return GL_TEXTURE4;
    case 5:
        return GL_TEXTURE5;
    case 6:
        return GL_TEXTURE6;
    case 7:
        return GL_TEXTURE7;
    case 8:
        return GL_TEXTURE8;
    default:
        Logger::LogError("Unknown texture slot");
        return 0;
    }
}

int GlShader::GetComponents(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT_MAT4:
        return 16;
    case GL_FLOAT:
        return 1;
    case GL_FLOAT_VEC2:
        return 2;
    case GL_FLOAT_VEC3:
        return 3;
    case GL_FLOAT_VEC4:
        return 4;
    default:
        Logger::LogError("Unknown type");
        return 0;
    }
}