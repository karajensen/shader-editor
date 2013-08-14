////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"

TextureManager::TextureManager(EnginePtr engine) :
    m_engine(engine)
{
}

void TextureManager::ClearTextureMap()
{
    m_textures.clear();
}

bool TextureManager::SetTexture(boost::property_tree::ptree::iterator& iterator, 
    const std::string& path, const std::string& textureType,
    ISceneNode* meshnode, int& textureSlot)
{
    if(iterator->second.count(textureType.c_str()) > 0)
    {
        ITexture* texture = nullptr;
        std::string textureName = iterator->second.get_child(textureType.c_str()).data();
        Texture_Map::iterator result = m_textures.find(textureName);

        if(result == m_textures.end())
        {
            // texture doesn't exist
            std::string texturePath = path + textureName;
            texture = m_engine->driver->getTexture(texturePath.c_str());

            if(texture)
            {
                // make sure texture is reloaded
                m_engine->driver->removeTexture(texture);
                texture = m_engine->driver->getTexture(texturePath.c_str());
            }

            if(!texture)
            {
                Logger::LogError(texturePath + " failed initilisation!");
                return false;
            }

            m_textures[textureName] = texture;
        }
        else
        {
            // texture exists
            texture = result->second;
        }

        meshnode->setMaterialTexture(textureSlot, texture);
        textureSlot++;
    }
    return true;
}