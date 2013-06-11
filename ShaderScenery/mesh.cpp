#include "mesh.h"

Mesh::Texture_Map Mesh::m_textures;

Mesh::Mesh() :
    m_usesCubeMapping(false),
    m_node(nullptr),
    m_materialIndex(NO_INDEX),
    m_specularity(0.0f),
    m_backfacecull(true)
{
}

Mesh::~Mesh()
{
    m_node = nullptr;
}

bool Mesh::Initialise(const std::string& path, const std::string& name, 
    int materialIndex, float specularity, bool backfacecull)
{
    m_name = name;
    m_materialIndex = materialIndex;
    m_specularity = specularity;
    m_backfacecull = backfacecull;

    IAnimatedMesh* mesh = Scene()->getMesh((path+name).c_str());
    if(!mesh)
    {
        Logger::LogError("Irrlicht GetMesh() failed");
        return false;
    }

    const int polysPerNode = 1024;
    m_node = Scene()->addOctreeSceneNode(mesh->getMesh(0), 0, -1, polysPerNode);
    if(!m_node)
    {
        Logger::LogError("Irrlicht AddOctreeSceneNode() failed");
        return false;
    }
   
    m_node->setName(m_name.c_str());
    SetShader(materialIndex);

    return true;
}

void Mesh::SetShader(int materialIndex)
{
    m_node->setMaterialType((video::E_MATERIAL_TYPE)materialIndex);
    m_node->setMaterialFlag(EMF_LIGHTING,false);
    for(unsigned int i = 0; i < m_node->getMaterialCount(); ++i)
    {
        m_node->getMaterial(i).Shininess = m_specularity;
        m_node->getMaterial(i).BackfaceCulling = m_backfacecull;
        m_node->getMaterial(i).Lighting = false;
    }
}

void Mesh::SetAssociatedShader()
{
    SetShader(m_materialIndex);
}

void Mesh::ForceReleaseMesh()
{
    if(m_node != nullptr)
    {
        m_node->remove();
    }
    m_node = nullptr;
}

void Mesh::ClearTextureMap()
{
    m_textures.clear();
}

bool Mesh::SetTexture(boost::property_tree::ptree::iterator& it, const std::string& path, 
    const std::string& textureType, int& textureSlot)
{
    if(it->second.count(textureType.c_str()) > 0)
    {
        ITexture* texture = nullptr;
        std::string textureName = it->second.get_child(textureType.c_str()).data();
        Texture_Map::iterator result = m_textures.find(textureName);

        if(result == m_textures.end())
        {
            // texture doesn't exist
            std::string texturePath = path + textureName;
            texture = Driver()->getTexture(texturePath.c_str());

            if(texture)
            {
                // make sure texture is reloaded
                Driver()->removeTexture(texture);
                texture = Driver()->getTexture(texturePath.c_str());
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

        m_node->setMaterialTexture(textureSlot, texture);
        textureSlot++;
    }
    return true;
}