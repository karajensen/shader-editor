////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include "common.h"
#include "matrix.h"

/**
* Irrlicht texture manager
*/
class TextureManager : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The irrlicht engine
    */
    explicit TextureManager(EnginePtr engine);

    /**
    * Set a texture for a mesh
    * @param iterator The iterator for the property tree for the particular mesh
    * @param path The path where the textures live
    * @param textureType The type of texture to load (eg. Diffuse, Normal)
    * @param meshnode The mesh to set the texture to
    * @param textureSlot Slot the texture will load into, increments if successful
    * @return whether creation succeeded
    */
    bool SetTexture(boost::property_tree::ptree::iterator& iterator,
        const std::string& path, const std::string& textureType, 
        ISceneNode* meshnode, int& textureSlot);

    /**
    * Clears the texture map of currently loaded textures
    */
    void ClearTextureMap();

private:

    typedef std::map<std::string, ITexture*> Texture_Map;

    EnginePtr m_engine;     ///< Irrlicht engine
    Texture_Map m_textures; ///< Map of currently loded textures
};