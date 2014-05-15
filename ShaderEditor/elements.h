////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

/**
* Individual texture object
*/
struct Texture
{
    /**
    * Type of texture
    */
    enum Type
    {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        MAX_TYPES
    };

    /**
    * Constructor
    */
    Texture();

    std::string name; ///< Name of the texture
    std::string path; ///< Path to the texture
};

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
    * Vertex Shader Attribute ordering
    */
    enum VertexAttribute
    {
        POSITION,
        UVS,
        NORMAL,
        COLOUR,
        TANGENT,
        BITANGENT
    };

    /**
    * Gets a text description of the shader component
    * @param component The component to query for text
    * @return the text description of the component
    */
    static std::string GetComponentDescription(unsigned int component);

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether the shader has the component
    */
    bool HasComponent(unsigned int component) const;

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

    bool backfacecull;            ///< Whether back facing polygons are culled
    int shaderIndex;              ///< Unique Index of the mesh shader
    float specularity;            ///< Brightness of the specular highlights
    std::string name;             ///< Name of the mesh
    std::vector<float> vertices;  ///< Mesh Vertex information
    std::vector<DWORD> indices;   ///< Mesh Index information
    std::vector<int> textureIDs;  ///< IDs for each texture used
    int vertexComponentCount;     ///< Number of components that make up a vertex
    int vertexCount;              ///< Number of vertices in the mesh
    int faceCount;                ///< Number of faces in the mesh
    int indexCount;               ///< Number of indicies in the mesh
    int maxTextures;              ///< Maximum textures used for this mesh
};