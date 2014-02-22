////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "elements.h"

Vertex::Vertex() : 
    x(0.0f),
    y(0.0f),
    z(0.0f),
    nx(0.0f),
    ny(0.0f),
    nz(0.0f),
    u(0.0f), 
    v(0.0f) 
{}

Light::Light() :
    castshadow(false),
    specularity(0.0f)
{
}

Shader::Shader() :
    index(NO_INDEX)
{
}

Mesh::Mesh() :
    specularity(0.0f),
    shaderIndex(NO_INDEX),
    backfacecull(true)
{
}