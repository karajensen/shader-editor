/****************************************************************
* Kara Jensen (mail@karajensen.com)
* Class for rendering screen space quads for post processing
*****************************************************************/
#pragma once

#include "common.h"

class Quad : public ISceneNode
{
public:

    Quad(ISceneNode* parent, int postShader, s32 id) : 
      ISceneNode(parent, Scene(), id),
      m_scaleMatrix(IdentityMatrix)
    {
        m_boundingbox.reset(0,0,0);

        dimension2d<u32> resolution = Driver()->getScreenSize();
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

    ~Quad()
    {
    }

    /**
    * Empty implementation
    */
    virtual void OnRegisterSceneNode() override
    {
    }

    /**
    * Empty implentation
    */
    virtual const aabbox3df& getBoundingBox() const override
    {
        return m_boundingbox;
    }

    /**
    * Overriding default rendering for scene node
    */
    virtual void render() override
    {
        IVideoDriver* driver = Driver();
        driver->setMaterial(m_material);
        driver->setTransform(ETS_PROJECTION, IdentityMatrix);
        driver->setTransform(ETS_VIEW, IdentityMatrix);
        driver->setTransform(ETS_WORLD, m_scaleMatrix);
        driver->drawIndexedTriangleList(&m_vertices[0], sm_vertexcount, &m_indices[0], sm_trianglecount);
    }

    /**
    * Set a texture for the quad to use
    * @param the texture to set
    * @param the slot in the shader the texture will be sent to
    */
    void SetTexture(ITexture* texture, u32 slot)
    {
        m_material.setTexture(slot, texture);
    }

private:
    
    static const int sm_vertexcount = 4;
    static const int sm_indexcount = 6;
    static const int sm_trianglecount = 2;

    matrix4 m_scaleMatrix;                         ///< Matrix used to flip the texture correctly
    SMaterial m_material;                          ///< Material used to render the quad
    aabbox3df m_boundingbox;                       ///< Required when inheriting from ISceneNode
    u16 m_indices[sm_indexcount];                  ///< Index buffer for the quad
    S3DVertex2TCoords m_vertices[sm_vertexcount];  ///< Vertices buffer for the quad
};
