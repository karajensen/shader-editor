////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include "cache.h"

Mesh::Mesh(const boost::property_tree::ptree& node) :
    MeshData(node)
{
    m_initialInstances = GetValueOptional(node, 1, "Instances");
    m_instances.resize(m_initialInstances);

    m_specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    m_ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    m_glow = GetValueOptional<float>(node, 0.0f, "Intensity");
    m_backfacecull = GetValueOptional<bool>(node, true, "BackfaceCulling");
}

void Mesh::Write(boost::property_tree::ptree& node) const
{
    MeshData::Write(node);

    AddValueOptional(node, "Intensity", m_glow, 0.0f);
    AddValueOptional(node, "Ambience", m_ambience, 1.0f);
    AddValueOptional(node, "Specularity", m_specularity, 0.0f);
    AddValueOptional(node, "BackfaceCulling", m_backfacecull ? 1 : 0, 1);
    AddValueOptional(node, "Instances", m_initialInstances, 1);
}

void Mesh::Write(Cache& cache)
{
    MeshData::Write(cache);

    cache.Mesh[MESH_AMBIENCE].SetUpdated(m_ambience);
    cache.Mesh[MESH_SPECULARITY].SetUpdated(m_specularity);
    cache.Mesh[MESH_GLOW].SetUpdated(m_glow);
    cache.MeshShader.SetUpdated(ShaderName());
}

void Mesh::Read(Cache& cache)
{
    MeshData::Read(cache);

    m_specularity = cache.Mesh[MESH_SPECULARITY].Get();
    m_ambience = cache.Mesh[MESH_AMBIENCE].Get();
    m_glow = cache.Mesh[MESH_GLOW].Get();
}

std::vector<Mesh::Instance>& Mesh::Instances()
{
    return m_instances;
}

const std::vector<Mesh::Instance>& Mesh::Instances() const
{
    return m_instances;
}

const float& Mesh::Specularity() const
{
    return m_specularity;
}

const float& Mesh::Ambience() const
{
    return m_ambience;
}

const float& Mesh::Glow() const
{
    return m_glow;
}

bool Mesh::BackfaceCull() const
{
    return m_backfacecull;
}