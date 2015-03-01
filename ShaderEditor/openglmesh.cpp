////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"

GlMeshData::GlMeshData(const std::string& name) :
    m_name(name)
{
}

GlWater::GlWater(const Water& water) :
    GlMeshData(water.Name()),
    m_water(water)
{
    m_vertexCount = water.Vertices().size();
    m_indexCount = water.Indices().size();
    m_vertices = water.Vertices();
    m_indices = water.Indices();
    m_name = water.Name();
}

GlMesh::GlMesh(const Mesh& mesh, PreRenderMesh preRender) :
    GlMeshData(mesh.Name()),
    m_mesh(mesh),
    m_preRender(preRender)
{
    m_vertexCount = mesh.Vertices().size();
    m_indexCount = mesh.Indices().size();
    m_vertices = mesh.Vertices();
    m_indices = mesh.Indices();
    m_name = mesh.Name();
}

GlQuad::GlQuad(const std::string& name) :
    GlMeshData(name)
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

    m_vertexCount = m_vertices.size();
    m_indexCount = m_indices.size();
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

bool GlMeshData::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STREAM_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_indices.size(), 
        &m_indices[0], GL_STREAM_DRAW);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_name + " buffers");
        return false;
    }

    m_initialised = true;
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
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

const Mesh& GlMesh::GetMesh() const
{
    return m_mesh;
}

const Water& GlWater::GetWater() const
{
    return m_water;
}

void GlMesh::Render()
{
    for (const Mesh::Instance& instance : m_mesh.Instances())
    {
        if (instance.shouldRender)
        {
            glm::mat4 scale;
            scale[0][0] = instance.scale;
            scale[1][1] = instance.scale;
            scale[2][2] = instance.scale;

            glm::mat4 translate;
            translate[3][0] = instance.position.x;
            translate[3][1] = instance.position.y;
            translate[3][2] = instance.position.z;
            
            glm::mat4 rotate;
            if (!instance.rotation.IsZero())
            {
                glm::mat4 rotateX, rotateY, rotateZ;
                glm::rotate(rotateX, instance.rotation.x, glm::vec3(1,0,0));
                glm::rotate(rotateY, instance.rotation.y, glm::vec3(0,1,0));
                glm::rotate(rotateZ, instance.rotation.z, glm::vec3(0,0,1));
                rotate = rotateZ * rotateX * rotateY;
            }

            m_preRender(translate * rotate * scale, instance.colour);
            GlMeshData::Render();
        }
    }
}
