////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"
#include "cache.h"

MeshData::MeshData(const std::string& name, 
                   const std::string& shaderName,
                   int shaderID) :
    m_name(name),
    m_shaderName(shaderName),
    m_shaderIndex(shaderID)
{
    m_textureIDs.resize(MAX_SLOTS);
    m_textureIDs.assign(MAX_SLOTS, NO_INDEX);
}

std::string MeshData::GetTypeDescription(unsigned int type)
{
    switch (type)
    {
    case SLOT_DIFFUSE:
        return "DiffuseMap";
    case SLOT_NORMAL:
        return "NormalMap";
    case SLOT_SPECULAR:
        return "SpecularMap";
    case SLOT_ENVIRONMENT:
        return "EnvironmentMap";
    case SLOT_CAUSTICS:
        return "CausticsMap";
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

void MeshData::SetSkyBox()
{
    m_skybox = true;
}

void MeshData::BackfaceCull(bool value)
{
    m_backfacecull = value;
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

int MeshData::VertexComponentCount() const
{
    return m_vertexComponentCount;
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
                            const BoundingArea& bounds)
{
    const float scale = std::max(std::max(instance.scale.x, instance.scale.y), instance.scale.z);
    const Float3 centerToMesh = instance.position - bounds.center;
    return centerToMesh.Length() <= (m_radius * scale) + bounds.radius;
}

void MeshData::PostTick()
{
    for (auto& instance : m_instances)
    {
        instance.requiresUpdate = false;
    }
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
                instance.requiresUpdate = true;
            }

            instance.render = ShouldRender(instance, cameraBounds);

            if (instance.render)
            {
                ++m_visibleInstances;
            }

            UpdateTransforms(instance);
        }
    }
}

bool MeshData::UsesCaustics() const
{
    return m_textureIDs[SLOT_CAUSTICS] != NO_INDEX;   
}

std::string MeshData::GetRenderedInstances() const
{
    return std::to_string(m_visibleInstances) + " / " +
        std::to_string(m_instances.size());
}

void MeshData::AddInstances(int amount)
{
    for (int i = 0; i < amount; ++i)
    {
        m_instances.emplace_back();
    }
}

void MeshData::SetInstance(int index,
                           const Float3& position,
                           const Float3& rotation,
                           float scale)
{
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale.x = scale;
    m_instances[index].scale.y = scale;
    m_instances[index].scale.z = scale;
    m_instances[index].requiresUpdate = true;
}

const Matrix& MeshData::GetWorldInstance(int instance)
{
    UpdateTransforms(m_instances[instance]);
    return m_instances[instance].world;
}

void MeshData::UpdateTransforms(Instance& instance)
{
    if (instance.requiresUpdate)
    {
        instance.world.MakeIdentity();

        if (instance.rotation.IsZero())
        {
            instance.world.m11 = instance.scale.x;
            instance.world.m12 = 0.0f;
            instance.world.m13 = 0.0f;
            instance.world.m21 = 0.0f;
            instance.world.m22 = instance.scale.y;
            instance.world.m23 = 0.0f;
            instance.world.m31 = 0.0f;
            instance.world.m32 = 0.0f;
            instance.world.m33 = instance.scale.z;
            instance.world.m14 = instance.position.x;
            instance.world.m24 = instance.position.y;
            instance.world.m34 = instance.position.z;            
        }
        else
        {
            Matrix scale;
            scale.m11 = instance.scale.x;
            scale.m22 = instance.scale.y;
            scale.m33 = instance.scale.z;
            
            Matrix translate;
            translate.m14 = instance.position.x;
            translate.m24 = instance.position.y;
            translate.m34 = instance.position.z;
            
            Matrix rotateX = Matrix::CreateRotateX(DegToRad(instance.rotation.x));
            Matrix rotateY = Matrix::CreateRotateY(DegToRad(instance.rotation.y));
            Matrix rotateZ = Matrix::CreateRotateZ(DegToRad(instance.rotation.z));
            
            instance.world = translate * (rotateZ * rotateX * rotateY) * scale;
        }
    }
}