/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* 
*****************************************************************/
#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include "common.h"
#include "matrix.h"

class Mesh : boost::noncopyable
{
public:

    Mesh();
    ~Mesh();

    /**
    * Initialises the Mesh
    * @param the path to the mesh
    * @param name the name of the Mesh
    * @param name the name of the shader
    * @param the shinyness of the material
    * @param whether to use backface culling or not
    * @return whether or not initialisation succeeded
    */
    bool Initialise(const std::string& path, const std::string& name, 
        int materialIndex, float specularity, bool backfacecull);

    /**
    * Sets the shader for the Mesh, will not set as the associated shader for the mesh
    * @param The irrlicht internal index of the shader the mesh wishes to use
    */
    void SetShader(int materialIndex);

    /**
    * Resets the current shader attached to the mesh to the one associated 
    */
    void SetAssociatedShader();

    /**
    * @return The name of the Mesh
    */
    const std::string& GetName() const { return m_name; }

    /**
    * Releases the mesh for reloading purposes
    */
    void ForceReleaseMesh();

    /**
    * @return the node for the mesh
    */
    const ISceneNode* GetMeshNode() const { return m_node; }

    /**
    * Set a texture for a mesh
    * @param the iterator for the property tree for the particular mesh
    * @param the path where the textures live
    * @param the type of texture to load (eg. Diffuse, Normal)
    * @param the slot the texture will load into for the shader, 
    *        will increment if loaded successfully
    * @return whether creation succeeded
    */
    bool SetTexture(boost::property_tree::ptree::iterator& it,
        const std::string& path, const std::string& textureType, int& textureSlot);

    /**
    * Clears the texture map of currently loaded textures
    */
    static void ClearTextureMap();

private:

    typedef std::map<std::string, ITexture*> Texture_Map;
    static Texture_Map m_textures;   ///< Map of currently loded textures

    std::string m_name;       ///< The name of the Mesh
    int m_materialIndex;      ///< The irrlicht index representing the associated saved
    bool m_usesCubeMapping;   ///< Whether the mesh uses cube mapping
    Matrix m_world;           ///< World matrix for Mesh
    float m_specularity;      ///< Shinyness of the mesh
    bool m_backfacecull;      ///< Whether to use backfaceculling
    ISceneNode* m_node;       ///< Irrlicht Scene node for the mesh

};