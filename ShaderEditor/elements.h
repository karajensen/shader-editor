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

    /**
    * All shader components
    */
    enum Component
    {
        FLAT,
        BUMP,
        SPECULAR,
        ALPHA,
        PARALLAX,
        MAX_COMPONENTS
    };

    /**
    * Gets a text description of the shader component
    * @param component The component to query for text
    * @return the text description of the component
    */
    static std::string GetComponentDescription(unsigned int component);

    int index;                         ///< Unique index of the shader
    std::string name;                  ///< name of the shader
    std::string glslVertexFile;        ///< filename of the glsl shader
    std::string glslFragmentFile;      ///< filename of the glsl shader
    std::string hlslShaderFile;        ///< filename of the hlsl shader
    std::vector<Component> components; ///< Sections that make up this shader
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
* Inidividual mesh object
*/
struct Mesh
{
    /**
    * Constructor
    */
    Mesh();

    bool backfacecull;                   ///< Whether back facing polygons are culled
    int shaderIndex;                     ///< Index of which shader to use
    float specularity;                   ///< Brightness of the specular highlights
    std::string name;                    ///< Name of the mesh
    std::vector<Float3> position;        ///< Position Vertex information
    std::vector<Float3> normals;         ///< Normal Vertex information
    std::vector<Float3> tangent;         ///< Tangent Vertex information
    std::vector<Float3> bitangent;       ///< Bitangent Vertex information
    std::vector<Colour> colour;          ///< Colour Vertex information
    std::vector<Float2> uvs;             ///< UV Texture Vertex information
    std::vector<DWORD> indices;          ///< Mesh Index information
};
