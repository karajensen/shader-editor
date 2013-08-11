////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"

/**
* Renders screen space quads for post processing
*/
class Quad : public ISceneNode
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    * @param parent The node of the parent
    * @param postShader The ID for the post shader
    * @param id The ID of the scene node
    */
    Quad(EnginePtr engine, ISceneNode* parent, int postShader, s32 id);

    /**
    * Destructor
    */
    ~Quad();

    /**
    * Not implemented
    */
    virtual void OnRegisterSceneNode() override;

    /**
    * Not implemented
    * @return the bounding box for the node
    */
    virtual const aabbox3df& getBoundingBox() const override;

    /**
    * Overriding default rendering for scene node
    */
    virtual void render() override;

    /**
    * Set a texture for the quad to use
    * @param texture The texture to set
    * @param slot The slot in the shader the texture will be sent to
    */
    void SetTexture(ITexture* texture, u32 slot);

private:

    static const int VERTEX_COUNT = 4;
    static const int INDEX_COUNT = 6;
    static const int TRIANGLE_COUNT = 2;

    matrix4 m_scaleMatrix;                       ///< Matrix used to flip the texture correctly
    SMaterial m_material;                        ///< Material used to render the quad
    aabbox3df m_boundingbox;                     ///< Required when inheriting from ISceneNode
    u16 m_indices[INDEX_COUNT];                  ///< Index buffer for the quad
    S3DVertex2TCoords m_vertices[VERTEX_COUNT];  ///< Vertices buffer for the quad
    EnginePtr m_engine;                          ///< Irrlicht engine
};
