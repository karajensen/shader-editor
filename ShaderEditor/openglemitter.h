////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglemitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglmesh.h"

/**
* OpenGL Emitter
*/
class GlEmitter
{
public:

    /**
    * Constructor for an emitter
    * @param emitter The emitter to use as a template
    * @param preRender a callback to setup rendering
    */
    GlEmitter(const Emitter& emitter, 
              PreRenderParticle preRender);

    /**
    * Renders the emitter
    * @param cameraPosition The world position of the camera
    * @param camerUp The up vector of the camera
    */
    void Render(const glm::vec3& cameraPosition,
                const glm::vec3& cameraUp);

    /**
    * Pre-Renders the emitter
    */
    void PreRender();

    /**
    * Initialises the emitter
    * @return whether initialisation succeeded
    */
    bool Initialise();

    /**
    * @return the unique ID for the mesh shader
    */
    int GetShaderID() const;

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
    std::unique_ptr<GlQuad> m_particle;      ///< Particle quad
    const Emitter& m_emitter;                ///< Emitter information
};
                                                                                                      