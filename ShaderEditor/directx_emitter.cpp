////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directx_emitter.h"

DxEmitter::DxEmitter(const Emitter& emitter, 
                     PreRenderParticle preRenderParticle) :

    m_emitter(emitter),
    m_particle(new DxQuad(emitter.Name())),
    m_preRender(preRenderParticle)
{
}

void DxEmitter::Render(ID3D11DeviceContext* context,
                       const D3DXVECTOR3& cameraPosition,
                       const D3DXVECTOR3& cameraUp)
{
    D3DXMATRIX scale, rotate, translate;
    D3DXMatrixIdentity(&scale);
    D3DXMatrixIdentity(&rotate);
    D3DXMatrixIdentity(&translate);

    for (const auto& instance : m_emitter.Instances())
    {
        if (instance.render)
        {
            for (const Particle& particle : instance.particles)
            {
                if (particle.Alive())
                {
                    // Particle always facing the camera
                    D3DXVECTOR3 right, up, forward;
                    forward.x = cameraPosition.x - particle.Position().x;
                    forward.y = cameraPosition.y - particle.Position().y;
                    forward.z = cameraPosition.z - particle.Position().z;
                    D3DXVec3Normalize(&forward, &forward);
                    D3DXVec3Cross(&right, &forward, &cameraUp);
                    D3DXVec3Cross(&up, &forward, &right);

                    scale._11 = particle.Size();
                    scale._22 = particle.Size();
                    scale._33 = particle.Size();

                    rotate._11 = right.x;
                    rotate._12 = right.y;
                    rotate._13 = right.z;
                    rotate._21 = up.x;
                    rotate._22 = up.y;
                    rotate._23 = up.z;
                    rotate._31 = forward.x;
                    rotate._32 = forward.y;
                    rotate._33 = forward.z;

                    translate._41 = particle.Position().x;
                    translate._42 = particle.Position().y;
                    translate._43 = particle.Position().z;

                    m_preRender(scale * rotate * translate, particle);
                    m_particle->Render(context);
                }
            }
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