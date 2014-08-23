////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - elements.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

struct PostProcessing
{
    /**
    * Post shader textures
    */
    enum Map
    {
        SCENE_MAP,
        NORMAL_MAP,
        DEPTH_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing()
    {
        SetPostTexture(SCENE_MAP);
    }

    /**
    * Sets which post texture should be rendered
    * @param map The post texture to render
    */
    void SetPostTexture(Map map)
    {
        alpha.assign(0.0f);
        alpha[map] = 1.0f;
    }

    /**
    * @param map the texture type to convert
    * @return the string name of the texture type
    */
    static std::string GetMapName(Map map)
    {
        switch (map)
        {
        case SCENE_MAP:
            return "Scene";
        case NORMAL_MAP:
            return "Normal Map";
        case DEPTH_MAP:
            return "Depth Map";
        default:
            return "None";
        }
    }

    float depthNear = 50.0f;             ///< Value where depth colour is min
    float depthFar = 400.0f;             ///< Value where depth colour is max
    Colour minimumColour = Colour(0.0f); ///< Colour ranges for RGB where A is the overall range
    Colour maximumColour = Colour(1.0f); ///< Colour ranges for RGB where A is the overall range
    std::array<float, MAX_MAPS> alpha;   ///< Visibility of post textures
};

struct Texture
{
    /**
    * Type of texture
    * Required in order of usage in shader body for OpenGL
    */
    enum Type
    {
        DIFFUSE,
        NORMAL,
        SPECULAR,
        MAX_TYPES
    };

    std::string name; ///< Name of the texture
    std::string path; ///< Path to the texture
};

struct Shader
{
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
    static std::string GetComponentDescription(unsigned int component)
    {
        switch (component)
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

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether the shader has the component
    */
    bool HasComponent(unsigned int component) const
    {
        return std::find(components.begin(), components.end(),
            Component(component)) != components.end();
    }

    int index = NO_INDEX;              ///< Unique index of the shader
    std::string name;                  ///< name of the shader
    std::string glslVertexFile;        ///< filename of the glsl shader
    std::string glslFragmentFile;      ///< filename of the glsl shader
    std::string hlslShaderFile;        ///< filename of the hlsl shader
    std::vector<Component> components; ///< Sections that make up this shader
};

struct Light
{
    std::string name;          ///< Name of the light
    Colour diffuse;            ///< Colour of the light
    Colour specular;           ///< Specular highlights the light will cast
    Float3 attenuation;        ///< How much the light will fade in distance
    Float3 position;           ///< World coordinates of the light
	float specularity = 1.0f;  ///< Brightness of the specular highlights
};

struct Mesh
{
    /**
    * Constructor
    */
    Mesh()
    {
        textureIDs.resize(Texture::MAX_TYPES);
        textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
    }

    bool backfacecull = true;      ///< Whether back facing polygons are culled
    int shaderIndex = NO_INDEX;    ///< Unique Index of the mesh shader
    float specularity = 1.0f;      ///< Brightness of the specular highlights
    float ambience = 1.0f;         ///< Ambient light multiplier
    float bump = 1.0f;             ///< Saturation of bump
    std::string name;              ///< Name of the mesh
    std::vector<float> vertices;   ///< Mesh Vertex information
    std::vector<DWORD> indices;    ///< Mesh Index information
    std::vector<int> textureIDs;   ///< IDs for each texture used
    int vertexComponentCount = 1;  ///< Number of components that make up a vertex
    int vertexCount = 0;           ///< Number of vertices in the mesh
    int faceCount = 0;             ///< Number of faces in the mesh
    int indexCount = 0;            ///< Number of indicies in the mesh
    int maxTextures = 0;           ///< Maximum textures used for this mesh
};