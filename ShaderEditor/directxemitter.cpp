////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxemitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxemitter.h"

DxEmitter::DxEmitter(const Emitter& emitter, 
                     PreRenderParticle preRenderParticle) :

    m_emitter(emitter),
    m_particle(new DxQuad(emitter.name)),
    m_preRender(preRenderParticle)
{
}

void DxEmitter::Render(ID3D11DeviceContext* context,
                       const D3DXVECTOR3& cameraPosition,
                       const D3DXVECTOR3& cameraUp)
{
    for (const Particle& particle : m_emitter.particles)
    {
        if (particle.alive)
        {
            // Particle always facing the camera
            D3DXVECTOR3 right, up, forward;
            forward.x = cameraPosition.x - particle.position.x;
            forward.y = cameraPosition.y - particle.position.y;
            forward.z = cameraPosition.z - particle.position.z;
            D3DXVec3Normalize(&forward, &forward);
            D3DXVec3Cross(&right, &forward, &cameraUp);
            D3DXVec3Cross(&up, &forward, &right);

            D3DXMATRIX scale;
            D3DXMatrixIdentity(&scale);
            scale._11 = particle.size;
            scale._22 = particle.size;
            scale._33 = particle.size;

            D3DXMATRIX rotate;
            D3DXMatrixIdentity(&rotate);
            rotate._11 = right.x;
            rotate._12 = right.y;
            rotate._13 = right.z;
            rotate._21 = up.x;
            rotate._22 = up.y;
            rotate._23 = up.z;
            rotate._31 = forward.x;
            rotate._32 = forward.y;
            rotate._33 = forward.z;

            D3DXMATRIX translate;
            D3DXMatrixIdentity(&translate);
            translate._41 = particle.position.x;
            translate._42 = particle.position.y;
            translate._43 = particle.position.z;
            
            m_preRender(scale * rotate * translate, particle);
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