////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "colour.h"

/**
* Mesh information
*/
struct MeshData
{
    MeshData();
    virtual ~MeshData() = default;

    /**
    * Fills in mesh data
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresBumpMapping Whether this mesh requires tangents/bitangents
    * @return whether creation was successful
    */
    bool Initialise(bool requiresNormals, bool requiresBumpMapping);

    float bump = 1.0f;                  ///< Saturation of bump
    std::string name;                   ///< Name of the mesh
    int shaderIndex = -1;               ///< Unique Index of the mesh shader to use
    int normalIndex = -1;               ///< Unique Index of the normal shader to use
    std::vector<float> vertices;        ///< Mesh Vertex information
    std::vector<unsigned long> indices; ///< Mesh Index information
    std::vector<int> textureIDs;        ///< IDs for each texture used
    int vertexComponentCount = 1;       ///< Number of components that make up a vertex
    int vertexCount = 0;                ///< Number of vertices in the mesh
    int faceCount = 0;                  ///< Number of faces in the mesh
    int indexCount = 0;                 ///< Number of indicies in the mesh
    int maxTextures = 0;                ///< The number of valid textures using
};

/**
* Mesh object rendered in the scene
*/
struct Mesh : public MeshData
{
    float specularity = 1.0f;        ///< Brightness of the specular highlights
    float ambience = 1.0f;           ///< Ambient light multiplier
    float glow = 1.0f;               ///< Intensity glow multiplier
    bool backfacecull = true;        ///< Whether back facing polygons are culled
    bool isInstanced = false;        ///< Whether this mesh using instancing

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        Colour colour;            ///< Colouring of the mesh
        Float3 position;          ///< Position offset
        float scale = 1.0f;       ///< Scaling of the mesh
        bool shouldRender = true; ///< Whether to render this instance
    };

    std::vector<Instance> instances; ///< Instances of this mesh
};