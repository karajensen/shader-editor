////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - quad.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "quad.h"

Quad::Quad(EnginePtr engine, ISceneNode* parent, int postShader, s32 id) : 
    ISceneNode(parent, engine->scene, id),
    m_scaleMatrix(IdentityMatrix),
    m_engine(engine)
{
    m_boundingbox.reset(0,0,0);

    dimension2d<u32> resolution = m_engine->driver->getScreenSize();
    float shiftX = 0.5f/resolution.Width;
    float shiftY = 0.5f/resolution.Height;
    SColor color(255,255,255,255);

    m_vertices[0] = S3DVertex2TCoords(
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            color, shiftX, 1.0f+shiftY, shiftX, 1.0f+shiftY);

    m_vertices[1] = S3DVertex2TCoords(
            1.0f, -1.0, 0.0f, 0.0f, 0.0f, -1.0f,
            color, 1.0f+shiftX, 1.0f+shiftY, 1.0f+shiftX, 1.0f+shiftY);

    m_vertices[2] = S3DVertex2TCoords(
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            color, shiftX, shiftY, shiftX, shiftY);

    m_vertices[3] = S3DVertex2TCoords(
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            color, 1.0f+shiftX, shiftY, 1.0f+shiftX, shiftY);

    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;

    m_indices[3] = 3;
    m_indices[4] = 2;
    m_indices[5] = 1;

    m_scaleMatrix.setScale(vector3d<f32>(1.0f,-1.0f,1.0f));

    setAutomaticCulling(scene::EAC_OFF);
    m_material.Lighting = false;
    m_material.BackfaceCulling = false;
    m_material.MaterialType = (E_MATERIAL_TYPE)postShader;
}

Quad::~Quad()
{
}

void Quad::OnRegisterSceneNode()
{
}

const aabbox3df& Quad::getBoundingBox() const
{
    return m_boundingbox;
}

void Quad::render()
{
    IVideoDriver* driver = m_engine->driver;
    driver->setMaterial(m_material);
    driver->setTransform(ETS_PROJECTION, IdentityMatrix);
    driver->setTransform(ETS_VIEW, IdentityMatrix);
    driver->setTransform(ETS_WORLD, m_scaleMatrix);
    driver->drawIndexedTriangleList(&m_vertices[0], 
        VERTEX_COUNT, &m_indices[0], TRIANGLE_COUNT);
}

void Quad::SetTexture(ITexture* texture, u32 slot)
{
    m_material.setTexture(slot, texture);
}
