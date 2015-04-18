////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"

GlMeshData::GlMeshData(const std::string& name,
                       const std::vector<float>& vertices,
                       const std::vector<unsigned int>& indices) :
    m_name(name),
    m_vertices(vertices),
    m_indices(indices)
{
}

GlMeshData::GlMeshData(const MeshData& data, PreRenderMesh preRender) :
    m_name(data.Name()),
    m_vertices(data.Vertices()),
    m_indices(data.Indices()),
    m_preRender(preRender)
{
    m_world.resize(data.Instances().size());
}

GlWater::GlWater(const Water& water, PreRenderMesh preRender) :
    GlMeshData(water, preRender),
    m_water(water)
{
}

GlTerrain::GlTerrain(const Terrain& terrain, PreRenderMesh preRender) :
    GlMeshData(terrain, preRender),
    m_terrain(terrain)
{
}

GlMesh::GlMesh(const Mesh& mesh, PreRenderMesh preRender) :
    GlMeshData(mesh, preRender),
    m_mesh(mesh)
{
}

GlQuad::GlQuad(const std::string& name) :
    GlMeshData(name, m_vertices, m_indices)
{
    // Top left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(-1.0f);  // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(0.0f);  // v

    // Top right corner
    m_vertices.emplace_back(1.0f); // x
    m_vertices.emplace_back(-1.0f); // y
    m_vertices.emplace_back(0.0f); // z
    m_vertices.emplace_back(1.0f); // u
    m_vertices.emplace_back(0.0f); // v

    // Bot right corner
    m_vertices.emplace_back(1.0f);  // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(1.0f);  // u
    m_vertices.emplace_back(1.0f); // v

    // Bot left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(1.0f); // v

    m_indices.emplace_back(0);
    m_indices.emplace_back(3);
    m_indices.emplace_back(1);
              
    m_indices.emplace_back(1);
    m_indices.emplace_back(3);
    m_indices.emplace_back(2);
}

GlMeshData::~GlMeshData()
{
    Release();
}

void GlMeshData::Release()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

bool GlMeshData::FillBuffers()
{
    glBindVertexArray(m_vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_indices.size(), 
        &m_indices[0], GL_STATIC_DRAW);

    return !HasCallFailed();
}

bool GlMeshData::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(1, &m_vboID);
    glGenBuffers(1, &m_iboID);
    m_initialised = true;
    m_updateInstances = true;

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: " + m_name + " Failed to generate buffers");
        return false;
    }

    if (!FillBuffers())
    {
        Logger::LogError("OpenGL: " + m_name + " Failed buffers");
        return false;
    }
    return true;
}

void GlMeshData::PreRender()
{
    assert(m_initialised);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void GlMeshData::Render()
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

const Mesh& GlMesh::GetMesh() const
{
    return m_mesh;
}

const Water& GlWater::GetWater() const
{
    return m_water;
}

const Terrain& GlTerrain::GetTerrain() const
{
    return m_terrain;
}

bool GlTerrain::Reload()
{
    return FillBuffers();
}

void GlMesh::Render()
{
    assert(m_world.size() == m_mesh.Instances().size());
    RenderInstances(m_mesh.Instances());
    m_updateInstances = false;
}

void GlTerrain::Render()
{
    assert(m_world.size() == m_terrain.Instances().size());
    RenderInstances(m_terrain.Instances());
    m_updateInstances = false;
}

void GlWater::Render()
{
    assert(m_world.size() == m_water.Instances().size());
    RenderInstances(m_water.Instances());
    m_updateInstances = false;
}

void GlMeshData::RenderInstances(const std::vector<MeshData::Instance>& instances)
{
    for (unsigned int i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];
        if (instance.requiresUpdate || m_updateInstances)
        {
            if (!instance.rotation.IsZero())
            {
                glm::mat4 scale;
                scale[0][0] = instance.scale.x;
                scale[1][1] = instance.scale.y;
                scale[2][2] = instance.scale.z;

                glm::mat4 translate;
                translate[3][0] = instance.position.x;
                translate[3][1] = instance.position.y;
                translate[3][2] = instance.position.z;
            
                glm::mat4 rotateX, rotateY, rotateZ;
                glm::rotate(rotateX, instance.rotation.x, glm::vec3(1,0,0));
                glm::rotate(rotateY, instance.rotation.y, glm::vec3(0,1,0));
                glm::rotate(rotateZ, instance.rotation.z, glm::vec3(0,0,1));

                m_world[i] = translate * (rotateZ * rotateX * rotateY) * scale;
            }
            else
            {
                auto& world = m_world[i];
                world[0][0] = instance.scale.x;
                world[0][1] = 0.0f;
                world[0][2] = 0.0f;
                world[1][0] = 0.0f;
                world[1][1] = instance.scale.y;
                world[1][2] = 0.0f;
                world[2][0] = 0.0f;
                world[2][1] = 0.0f;
                world[2][2] = instance.scale.z;
                world[3][0] = instance.position.x;
                world[3][1] = instance.position.y;
                world[3][2] = instance.position.z;
            }
        }

        if (instance.enabled && instance.render)
        {
            m_preRender(m_world[i]);
            GlMeshData::Render();
        }
    }
}
