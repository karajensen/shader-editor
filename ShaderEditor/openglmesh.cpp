////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"

GlMeshBuffer::GlMeshBuffer(const std::string& name,
                           const std::vector<float>& vertices,
                           const std::vector<unsigned int>& indices) :
    m_name(name),
    m_vertices(vertices),
    m_indices(indices)
{
}

GlMeshData::GlMeshData(const MeshData& data, PreRenderMesh preRender) :
    GlMeshBuffer(data.Name(), data.Vertices(), data.Indices()),
    m_preRender(preRender),
    m_meshdata(data)
{
}

GlMeshData::GlMeshData(const MeshData& mesh,
                       const std::vector<float>& vertices,
                       const std::vector<unsigned int>& indices,
                       PreRenderMesh preRender) :

    GlMeshBuffer(mesh.Name(), vertices, indices),
    m_meshdata(mesh),
    m_preRender(preRender)
{
}

GlMesh::GlMesh(const MeshData& mesh, PreRenderMesh preRender) :
    GlMeshData(mesh, preRender)
{
}

GlMeshBuffer::~GlMeshBuffer()
{
    Release();
}

GlQuadData::GlQuadData()
{
    // Top left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(-1.0f);  // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f); // u
    vertices.emplace_back(0.0f);  // v

    // Top right corner
    vertices.emplace_back(1.0f); // x
    vertices.emplace_back(-1.0f); // y
    vertices.emplace_back(0.0f); // z
    vertices.emplace_back(1.0f); // u
    vertices.emplace_back(0.0f); // v

    // Bot right corner
    vertices.emplace_back(1.0f);  // x
    vertices.emplace_back(1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(1.0f);  // u
    vertices.emplace_back(1.0f); // v

    // Bot left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f); // u
    vertices.emplace_back(1.0f); // v

    indices.emplace_back(0);
    indices.emplace_back(3);
    indices.emplace_back(1);
            
    indices.emplace_back(1);
    indices.emplace_back(3);
    indices.emplace_back(2);
}

void GlMeshBuffer::Release()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

bool GlMeshBuffer::FillBuffers()
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
    m_updateInstances = true;
    m_world.clear();
    m_world.resize(m_meshdata.Instances().size());
    return GlMeshBuffer::Initialise();
}

bool GlMeshBuffer::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(1, &m_vboID);
    glGenBuffers(1, &m_iboID);
    m_initialised = true;

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

void GlMeshBuffer::PreRender()
{
    assert(m_initialised);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void GlMeshBuffer::Render()
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

const MeshData& GlMeshData::GetData() const
{
    return m_meshdata;
}

const Mesh& GlMesh::GetMesh() const
{
    return static_cast<const Mesh&>(GetData());
}

const Water& GlMesh::GetWater() const
{
    return static_cast<const Water&>(GetData());
}

const Terrain& GlMesh::GetTerrain() const
{
    return static_cast<const Terrain&>(GetData());
}

bool GlMeshBuffer::Reload()
{
    return FillBuffers();
}

void GlMeshData::Render()
{
    const auto& instances = m_meshdata.Instances();
    for (unsigned int i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];
        if (instance.requiresUpdate || m_updateInstances)
        {
            m_world[i][0][0] = instance.world.m11;  
            m_world[i][1][0] = instance.world.m12;
            m_world[i][2][0] = instance.world.m13;
            m_world[i][3][0] = instance.world.m14;

            m_world[i][0][1] = instance.world.m21;
            m_world[i][1][1] = instance.world.m22;
            m_world[i][2][1] = instance.world.m23;
            m_world[i][3][1] = instance.world.m24;

            m_world[i][0][2] = instance.world.m31;
            m_world[i][1][2] = instance.world.m32;
            m_world[i][2][2] = instance.world.m33;
            m_world[i][3][2] = instance.world.m34;
        }

        if (instance.enabled && instance.render)
        {
            m_preRender(m_world[i], instance.colour);
            GlMeshData::Render();
        }
    }
    m_updateInstances = false;
}

GlQuad::GlQuad(const std::string& name) :
    GlMeshBuffer(name, vertices, indices)
{
}

GlQuadMesh::GlQuadMesh(const MeshData& mesh, PreRenderMesh preRender) :
    GlMeshData(mesh, vertices, indices, preRender)
{
}