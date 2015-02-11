////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxemitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxemitter.h"

DxEmitter::DxEmitter(const Emitter& emitter, PreRenderQuad preRender) :
    m_emitter(emitter),
    m_particle(new DxQuad(emitter.name, preRender))
{
}

void DxEmitter::Render(ID3D11DeviceContext* context)
{
}

void DxEmitter::Release()
{
    m_particle->Release();
}

void DxEmitter::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_particle->Initialise(device, context);
}