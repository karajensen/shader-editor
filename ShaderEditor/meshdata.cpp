////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"
#include "cache.h"

MeshData::MeshData(const boost::property_tree::ptree& node)
{
    m_textureIDs.resize(MAX_SLOTS);
    m_textureNames.resize(MAX_SLOTS);
    m_textureIDs.assign(MAX_SLOTS, NO_INDEX);

    m_name = GetValue<std::string>(node, "Name");
    m_backfacecull = GetValueOptional<bool>(node, true, "BackfaceCulling");
    m_skybox = GetValueOptional<bool>(node, false, "SkyBox");
    m_shaderName = GetValueOptional<std::string>(node, "", "Shader");
    m_usesCaustics = GetValueOptional<bool>(node, false, "UseCaustics");

    for (int i = 0; i < MAX_SLOTS; ++i)
    {
        m_textureNames[i] = GetValueOptional(node, 
            std::string(), GetTypeDescription(i).c_str());
    }
}

void MeshData::Write(boost::property_tree::ptree& node) const
{
    node.add("Name", m_name);
    AddValueOptional(node, "Shader", m_shaderName, std::string());
    AddValueOptional(node, "BackfaceCulling", m_backfacecull ? 1 : 0, 1);
    AddValueOptional(node, "SkyBox", m_skybox ? 1 : 0, 0);

    for (int i = 0; i < MAX_SLOTS; ++i)
    {
        if (i != SLOT_CAUSTICS && m_textureIDs[i] != NO_INDEX)
        {
            node.add(GetTypeDescription(i), m_textureNames[i]);
        }
    }
}

std::string MeshData::GetTypeDescription(unsigned int type)
{
    switch (type)
    {
    case SLOT_DIFFUSE:
        return "Diffuse";
    case SLOT_NORMAL:
        return "Normal";
    case SLOT_SPECULAR:
        return "Specular";
    case SLOT_ENVIRONMENT:
        return "Environment";
    case SLOT_CAUSTICS:
        return "Caustics";
    default:
        return "None";
    };
}

void MeshData::InitialiseMeshData()
{
    // Assumes position is always first in a vertex
    for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
    {
        const Float3 position(m_vertices[vertex], 
            m_vertices[vertex + 1], m_vertices[vertex + 2]);
        m_radius = std::max(m_radius, position.Length());
    }
}

const std::string& MeshData::Name() const
{
    return m_name;
}

int MeshData::ShaderID() const
{
    return m_shaderIndex;
}

bool MeshData::BackfaceCull() const
{
    return m_backfacecull;
}

const std::vector<float>& MeshData::Vertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& MeshData::Indices() const
{
    return m_indices;
}

const std::vector<int>& MeshData::TextureIDs() const
{
    return m_textureIDs;
}

const std::vector<std::string>& MeshData::TextureNames() const
{
    return m_textureNames;
}

int MeshData::VertexComponentCount() const
{
    return m_vertexComponentCount;
}

void MeshData::SetShaderID(int shaderID)
{
    m_shaderIndex = shaderID;
}

void MeshData::SetTexture(TextureSlot slot, int ID)
{
    m_textureIDs[slot] = ID;
}

const std::string& MeshData::ShaderName() const
{
    return m_shaderName;
}

std::vector<MeshData::Instance>& MeshData::Instances()
{
    return m_instances;
}

const std::vector<MeshData::Instance>& MeshData::Instances() const
{
    return m_instances;
}

const MeshData::Instance& MeshData::GetInstance(int index) const
{
    return m_instances.at(index);
}

bool MeshData::ShouldRender(const Instance& instance,
                            const Float3& position, 
                            const BoundingArea& bounds)
{
    const float scale = std::max(std::max(instance.scale.x, instance.scale.y), instance.scale.z);
    const Float3 centerToMesh = instance.position - bounds.center;
    return centerToMesh.Length() <= (m_radius * scale) + bounds.radius;
}

void MeshData::Tick(const Float3& cameraPosition, 
                    const BoundingArea& cameraBounds,
                    int causticsTexture)
{
    if (UsesCaustics())
    {
        SetTexture(SLOT_CAUSTICS, causticsTexture);
    }
   
    m_visibleInstances = 0;
    for (auto& instance : m_instances)
    {
        if (instance.enabled)
        {
            if (m_skybox)
            {
                instance.position = cameraPosition;
            }

            instance.render = ShouldRender(
                instance, cameraPosition, cameraBounds);

            if (instance.render)
            {
                ++m_visibleInstances;
            }
        }
    }
}

bool MeshData::UsesCaustics() const
{
    return m_usesCaustics;
}

std::string MeshData::GetRenderedInstances() const
{
    return std::to_string(m_visibleInstances) + " / " +
        std::to_string(m_instances.size());
}