////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxemitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxemitter.h"

DxEmitter::DxEmitter(const Emitter& emitter, 
                     PreRenderQuad preRenderQuad, 
                     PreRenderParticle preRenderParticle) :

    m_emitter(emitter),
    m_particle(new DxQuad(emitter.name, preRenderQuad)),
    m_preRender(preRenderParticle)
{
}

void DxEmitter::Render(ID3D11DeviceContext* context)
{
    for (const Particle& particle : m_emitter.particles)
    {
        if (particle.alive)
        {
            m_preRender(particle);
            m_particle->SetTexture(particle.texture);
            m_particle->Render(context);
        }
    }
}

void DxEmitter::Release()
{
    m_particle->Release();
}

void DxEmitter::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_particle->Initialise(device, context);
}

const Emitter& DxEmitter::GetEmitter() const
{
    return m_emitter;
}