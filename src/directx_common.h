////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>

#pragma warning (disable : 4005) // For DX10
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "render_data.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "texture_procedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"

/**
* Callbacks for pre-rendering elements
*/
typedef std::function<void(const D3DXMATRIX&, const Particle&)> PreRenderParticle;
typedef std::function<void(const D3DXMATRIX&, int)> PreRenderMesh;

/**
* Sets the name of the directx object for debugging
* @param object The object to set
* @param name The name of the object
*/
template <typename T> void SetDebugName(T* object, const std::string& name)
{
    #ifdef _DEBUG
    if(object)
    {
        object->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
    }
    #endif
}

/**
* Releases the directx object safely
* @param pObject The pointer to the object to release
*/
template <typename T> void SafeRelease(T** pObject)
{
    if(*pObject)
    {
        (*pObject)->Release();
        *pObject = nullptr;
    }
}