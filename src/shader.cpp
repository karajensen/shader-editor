////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "shader.h"
#include "render_data.h"

#include <boost/algorithm/string.hpp>

namespace
{
    const std::string ASM_EXTENSION(".as");
    const std::string SHADER_EXTENSION(".fx");
    const std::string GLSL_VERTEX("_glsl_vert");
    const std::string GLSL_FRAGMENT("_glsl_frag");
    const std::string HLSL_SHADER("_hlsl");
    const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
    const std::string GENERATED_PATH(SHADER_PATH + "Generated//");
    const std::string BASE_SHADER("shader");
}

Shader::Shader(const std::string& name, 
               unsigned int components,
               bool fromFragments)
    : m_name(name)
    , m_components(components)
    , m_fromFragments(fromFragments)
{
}

bool Shader::GenerateFromFragments() const
{
    return m_fromFragments;
}

Shader::Component Shader::StringAsComponent(const std::string& component)
{
    if (boost::iequals(component, "CAUSTICS"))
    {
        return CAUSTICS;
    }
    if (boost::iequals(component, "FLAT"))
    {
        return FLAT;
    }
    if (boost::iequals(component, "BUMP"))
    {
        return BUMP;
    }
    if (boost::iequals(component, "SPECULAR"))
    {
        return SPECULAR;
    }
    return NONE;
}

std::string Shader::ComponentAsString(unsigned int component)
{
    switch (component)
    {
    case CAUSTICS:
        return "CAUSTICS";
    case FLAT:
        return "FLAT";
    case BUMP:
        return "BUMP";
    case SPECULAR:
        return "SPECULAR";
    default:
        return "NONE";
    };
}

bool Shader::HasComponent(Component component) const
{
    return (m_components & component) == component;
}

const std::string& Shader::Name() const
{
    return m_name;
}

unsigned int Shader::GetComponents() const
{
    return m_components;
}

std::string Shader::GLSLVertexBase() const
{
    return SHADER_PATH + (m_fromFragments ? BASE_SHADER : m_name) +
        GLSL_VERTEX + SHADER_EXTENSION;
}

std::string Shader::GLSLFragmentBase() const
{
    return SHADER_PATH + (m_fromFragments ? BASE_SHADER : m_name) +
        GLSL_FRAGMENT + SHADER_EXTENSION;
}

std::string Shader::HLSLShaderBase() const
{
    return SHADER_PATH + (m_fromFragments ? BASE_SHADER : m_name) +
        HLSL_SHADER + SHADER_EXTENSION;
}

std::string Shader::GLSLVertexFile() const
{
    return GENERATED_PATH + m_name + GLSL_VERTEX + SHADER_EXTENSION;
}

std::string Shader::GLSLFragmentFile() const
{
    return GENERATED_PATH + m_name + GLSL_FRAGMENT + SHADER_EXTENSION;
}

std::string Shader::HLSLShaderFile() const
{
    return GENERATED_PATH + m_name + HLSL_SHADER + SHADER_EXTENSION;
}

std::string Shader::GLSLVertexAsmFile() const
{
    return GENERATED_PATH + m_name + GLSL_VERTEX + ASM_EXTENSION;
}

std::string Shader::GLSLFragmentAsmFile() const
{
    return GENERATED_PATH + m_name + GLSL_FRAGMENT + ASM_EXTENSION;
}

std::string Shader::HLSLShaderAsmFile() const
{
    return GENERATED_PATH + m_name + HLSL_SHADER + ASM_EXTENSION;
}