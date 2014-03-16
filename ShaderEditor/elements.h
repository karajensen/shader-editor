////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

/**
* Individual shader object
*/
struct Shader
{
    /**
    * Constructor
    */
    Shader() :
        index(NO_INDEX)
    {
    }

    int index;                    ///< Unique index of the shader
    std::string name;             ///< name of the shader
    std::string glslVertexFile;   ///< filename of the glsl shader
    std::string glslFragmentFile; ///< filename of the glsl shader
    std::string hlslShaderFile;   ///< filename of the hlsl shader
};

/**
* Individual light object
*/
struct Light
{
    /**
    * Constructor
    */
    Light() :
        castshadow(false),
        specularity(0.0f)
    {
    }

    bool castshadow;     ///< Whether the light casts shadows or not
    std::string name;    ///< Name of the light
    Colour diffuse;      ///< Colour of the light
    Colour specular;     ///< Specular highlights the light will cast
    Float3 attenuation;  ///< How much the light will fade in distance
    Float3 position;     ///< World coordinates of the light
    float specularity;   ///< Brightness of the specular highlights
};

/**
* Aggregate Mesh Vertex structure
*/
struct Vertex
{
    /**
    * Constructor
    */
    Vertex() :
        x(0.0f),
        y(0.0f),
        z(0.0f),
        nx(0.0f),
        ny(0.0f),
        nz(0.0f),
        u(0.0f),
        v(0.0f)
    {
    }

    float x,y,z;     ///< Vertex position
    float nx,ny,nz;  ///< Vertex normal
    float u,v;       ///< Vertex uvs
};

/**
* Inidividual mesh object
*/
struct Mesh
{
    /**
    * Constructor
    */
    Mesh() :
        specularity(0.0f),
        shaderIndex(NO_INDEX),
        backfacecull(true)
    {
    }

    bool backfacecull;              ///< Whether back facing polygons are culled
    int shaderIndex;                ///< Index of which shader to use
    float specularity;              ///< Brightness of the specular highlights
    std::string name;               ///< Name of the mesh
    std::vector<Vertex> vertices;   ///< Vertex buffer
    std::vector<DWORD> indices;     ///< Index buffer
    Matrix world;                   ///< World matrix of the mesh
};
