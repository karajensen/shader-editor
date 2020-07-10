////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "colour.h"
#include "mesh_data.h"
#include "mesh_attributes.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData, public MeshAttributes
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    Mesh(const std::string& name, 
         const std::string& shaderName, 
         int shaderID);

    /**
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * Initialises the mesh data buffer containers from file
    * @param path The full path to the mesh file
    * @param uvScale The scale to apply to mesh UVs
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            const Float2& uvScale,
                            bool requiresNormals, 
                            bool requiresTangents);
};
