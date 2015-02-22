////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglemitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglemitter.h"

GlEmitter::GlEmitter(const Emitter& emitter, 
                     PreRenderParticle preRenderParticle) :

    m_emitter(emitter),
    m_particle(new GlQuad(emitter.name)),
    m_preRender(preRenderParticle)
{
}

int GlEmitter::GetShaderID() const
{
    return m_emitter.shaderIndex;
}

void GlEmitter::PreRender()
{
    m_particle->PreRender();
}

void GlEmitter::Render(const glm::vec3& cameraPosition,
                       const glm::vec3& cameraUp)
{
    for (const Particle& particle : m_emitter.particles)
    {
        if (particle.alive)
        {
            // Particle always facing the camera
            glm::vec3 right, up, forward;
            forward.x = cameraPosition.x - particle.position.x;
            forward.y = cameraPosition.y - particle.position.y;
            forward.z = cameraPosition.z - particle.position.z;
            
            forward = glm::normalize(forward);
            right = glm::cross(forward, cameraUp);
            up = glm::cross(forward, right);

            glm::mat4 scale;
            scale[0][0] = particle.size;  
            scale[1][1] = particle.size;
            scale[2][2] = particle.size;
            
            glm::mat4 rotate;
            rotate[0][0] = right.x;  
            rotate[0][1] = right.y;
            rotate[0][2] = right.z;
            rotate[1][0] = up.x;  
            rotate[1][1] = up.y;
            rotate[1][2] = up.z;
            rotate[2][0] = forward.x;  
            rotate[2][1] = forward.y;
            rotate[2][2] = forward.z;

            glm::mat4 translate;
            translate[3][0] = particle.position.x;
            translate[3][1] = particle.position.y;
            translate[3][2] = particle.position.z;
            
            m_preRender(translate * rotate * scale, particle);
            m_particle->Render();
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