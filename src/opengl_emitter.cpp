////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl_emitter.h"

GlEmitter::GlEmitter(const Emitter& emitter, 
                     PreRenderParticle preRenderParticle) :

    m_emitter(emitter),
    m_particle(new GlQuad(emitter.Name())),
    m_preRender(preRenderParticle)
{
}

int GlEmitter::GetShaderID() const
{
    return m_emitter.ShaderID();
}

void GlEmitter::PreRender()
{
    m_particle->PreRender();
}

void GlEmitter::Render(const glm::vec3& cameraPosition,
                       const glm::vec3& cameraUp)
{
    glm::mat4 scale, rotate, translate;

    for (const auto& instance : m_emitter.Instances())
    {
        if (instance.render)
        {
            for (const Particle& particle : instance.particles)
            {
                if (particle.Alive())
                {
                    // Particle always facing the camera
                    glm::vec3 right, up, forward;
                    forward.x = cameraPosition.x - particle.Position().x;
                    forward.y = cameraPosition.y - particle.Position().y;
                    forward.z = cameraPosition.z - particle.Position().z;

                    forward = glm::normalize(forward);
                    right = glm::cross(forward, cameraUp);
                    up = glm::cross(forward, right);

                    scale[0][0] = particle.Size();
                    scale[1][1] = particle.Size();
                    scale[2][2] = particle.Size();

                    rotate[0][0] = right.x;
                    rotate[0][1] = right.y;
                    rotate[0][2] = right.z;
                    rotate[1][0] = up.x;
                    rotate[1][1] = up.y;
                    rotate[1][2] = up.z;
                    rotate[2][0] = forward.x;
                    rotate[2][1] = forward.y;
                    rotate[2][2] = forward.z;

                    translate[3][0] = particle.Position().x;
                    translate[3][1] = particle.Position().y;
                    translate[3][2] = particle.Position().z;

                    m_preRender(translate * rotate * scale, particle);
                    m_particle->Render();
                }
            }
        }
    }
}

void GlEmitter::Release()
{
    m_particle->Release();
}

bool GlEmitter::Initialise()
{
    return m_particle->Initialise();
}

const Emitter& GlEmitter::GetEmitter() const
{
    return m_emitter;
}