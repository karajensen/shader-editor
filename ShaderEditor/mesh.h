////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "colour.h"
#include "matrix.h"
#include "meshdata.h"
#include "meshAttributes.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData, public MeshAttributes
{
public:

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    Mesh(const boost::property_tree::ptree& node);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const override;

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
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            bool requiresNormals, 
                            bool requiresTangents);
};