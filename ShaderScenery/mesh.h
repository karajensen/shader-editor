////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include "common.h"
#include "matrix.h"

/**
* Irrlicht mesh
*/
class Mesh : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit Mesh(EnginePtr engine);

    /**
    * Destructor
    */
    ~Mesh();

    /**
    * Initialises the Mesh
    * @param path The path to the mesh
    * @param name Name the name of the Mesh
    * @param materialIndex Name the name of the shader
    * @param specularity The shinyness of the material
    * @param backfacecull Whether to use backface culling or not
    * @return whether or not initialisation succeeded
    */
    bool Initialise(const std::string& path, const std::string& name, 
        int materialIndex, float specularity, bool backfacecull);

    /**
    * Sets the shader for the Mesh, will not set
    * as the associated shader for the mesh
    * @param materialIndex The irrlicht internal index of the shader
    */
    void SetShader(int materialIndex);

    /**
    * Resets the current shader attached to the mesh to the one associated 
    */
    void SetAssociatedShader();

    /**
    * @return The name of the Mesh
    */
    const std::string& GetName() const;

    /**
    * Releases the mesh for reloading purposes
    */
    void ForceReleaseMesh();

    /**
    * @return the node for the mesh
    */
    ISceneNode* GetMeshNode() const;

private:

    std::string m_name;       ///< The name of the Mesh
    int m_materialIndex;      ///< The irrlicht index representing the associated saved
    bool m_usesCubeMapping;   ///< Whether the mesh uses cube mapping
    Matrix m_world;           ///< World matrix for Mesh
    float m_specularity;      ///< Shinyness of the mesh
    bool m_backfacecull;      ///< Whether to use backfaceculling
    ISceneNode* m_node;       ///< Irrlicht Scene node for the mesh
    EnginePtr m_engine;       ///< Irrlicht engine
};