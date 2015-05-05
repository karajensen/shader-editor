////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "shader.h"
#include "renderdata.h"
#include "boost/algorithm/string.hpp"

Shader::Shader(const std::string& name, 
               const std::string& path,
               unsigned int components) :

    m_name(name),
    m_components(components)
{
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

const std::string& Shader::GLSLVertexFile() const
{
    return m_glslVertexFile;
}

const std::string& Shader::GLSLFragmentFile() const
{
    return m_glslFragmentFile;
}

const std::string& Shader::HLSLShaderFile() const
{
    return m_hlslShaderFile;
}

void Shader::GLSLVertexFile(const std::string& file)
{
    m_glslVertexFile = file;
}

void Shader::GLSLFragmentFile(const std::string& file)
{
    m_glslFragmentFile = file;
}

void Shader::HLSLShaderFile(const std::string& file)
{
    m_hlslShaderFile = file;
}

unsigned int Shader::GetComponents() const
{
    return m_components;
}