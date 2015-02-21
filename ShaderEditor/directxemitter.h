////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxemitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxmesh.h"

/**
* DirectX Emitter
*/
class DxEmitter
{
public:

    /**
    * Constructor for an emitter
    * @param emitter The emitter to use as a template
    * @param preRender a callback to setup rendering
    */
    DxEmitter(const Emitter& emitter, 
              PreRenderQuad preRender,
              PreRenderParticle preRenderParticle);

    /**
    * Renders the emitter
    * @param context Direct3D device context
    * @param cameraPosition The world position of the camera
    * @param camerUp The up vector of the camera
    */
    void Render(ID3D11DeviceContext* context, 
                const D3DXVECTOR3& cameraPosition,
                const D3DXVECTOR3& cameraUp);

    /**
    * Initialises the emitter
    * @param device The DirectX device interface
    * @param context Direct3D device context
    */
    void Initialise(ID3D11Device* device, ID3D11DeviceContext* context);

    /**
    * Releases the emitter
    */
    void Release();

    /**
    * @return the emitter information
    */
    const Emitter& GetEmitter() const;

private:

    PreRenderParticle m_preRender = nullptr; ///< Pre render callback for particles
    std::unique_ptr<DxQuad> m_particle;      ///< Particle quad
    const Emitter& m_emitter;                ///< Emitter information
};
                                                                                                      