////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"

Mesh::Mesh(EnginePtr engine) :
    m_materialIndex(NO_INDEX),
    m_specularity(0.0f),
    m_backfacecull(true),
    m_node(nullptr),
    m_engine(engine)
{
}

Mesh::~Mesh()
{
    m_node = nullptr;
}

const std::string& Mesh::GetName() const 
{ 
    return m_name;
}

ISceneNode* Mesh::GetMeshNode() const 
{
    return m_node;
}

bool Mesh::Initialise(const std::string& path, const std::string& name, 
    int materialIndex, float specularity, bool backfacecull)
{
    m_name = name;
    m_materialIndex = materialIndex;
    m_specularity = specularity;
    m_backfacecull = backfacecull;

    IAnimatedMesh* mesh = m_engine->scene->getMesh((path+name).c_str());
    if(!mesh)
    {
        Logger::LogError("Irrlicht GetMesh() failed");
        return false;
    }

    const int polysPerNode = 1024;
    m_node = m_engine->scene->addOctreeSceneNode(mesh->getMesh(0), 0, -1, polysPerNode);
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

void Mesh::SetWireframe(bool wireframe)
{
    m_node->setMaterialFlag(EMF_WIREFRAME, wireframe);
}