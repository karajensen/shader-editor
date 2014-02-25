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
    Shader();

    int index;                    ///< Unique index of the shader
    std::string name;             ///< name of the shader
    std::string glslVertexFile;   ///< filename of the glsl shader
    std::string glslFragmentFile; ///< filename of the glsl shader
    std::string hlslFragmentFile; ///< filename of the hlsl shader
};

/**
* Individual light object
*/
struct Light
{
    /**
    * Constructor
    */
    Light();

    bool castshadow;     ///< Whether the light casts shadows or not
    std::string name;    ///< Name of the light
    Colour diffuse;      ///< Colour of the light
    Colour specular;     ///< Specular highlights the light will cast
    Float3 attenuation;  ///< How much the light will fade in distance
    Float3 position;     ///< World coordinates of the light
    float specularity;   ///< Brightness of the specular highlights
};

/**
* Mesh Vertex structure
*/
struct Vertex
{
    /**
    * Contructor
    */
    Vertex();

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
    Mesh();

    bool backfacecull;              ///< Whether back facing polygons are culled
    int shaderIndex;                ///< Index of which shader to use
    float specularity;              ///< Brightness of the specular highlights
    std::string name;               ///< Name of the mesh
    std::vector<Vertex> vertices;   ///< Vertex buffer
    std::vector<DWORD> indices;     ///< Index buffer
    Matrix world;                   ///< World matrix of the mesh
};
