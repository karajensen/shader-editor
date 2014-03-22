////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "elements.h"

Shader::Shader() :
    index(NO_INDEX)
{
}

std::string Shader::GetComponentDescription(unsigned int component)
{
    switch(component)
    {
    case FLAT:
        return "FLAT";
    case BUMP:
        return "BUMP";
    case SPECULAR:
        return "SPECULAR";
    case ALPHA:
        return "ALPHA";
    case PARALLAX:
        return "PARALLAX";
    default:
        return "NONE";
    };
}

Light::Light() :
    castshadow(false),
    specularity(0.0f)
{
}

Mesh::Mesh() :
    specularity(0.0f),
    shaderIndex(NO_INDEX),
    backfacecull(true)
{
}