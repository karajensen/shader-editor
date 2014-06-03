////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning (disable : 4005)
#include "directx/include/d3d11.h"
#include "directx/include/d3dx11.h"
#include "directx/include/d3dx10.h"
#include "common.h"

const std::string POST_SHADER("post" + HLSL_SHADER);
const std::string POST_FX_PATH(SHADER_PATH + POST_SHADER + SHADER_EXTENSION);
const std::string POST_ASM_PATH(GENERATED_PATH + POST_SHADER + ASM_EXTENSION);

const std::string NORM_SHADER("normal" + HLSL_SHADER);
const std::string NORM_FX_PATH(SHADER_PATH + NORM_SHADER + SHADER_EXTENSION);
const std::string NORM_ASM_PATH(GENERATED_PATH + NORM_SHADER + ASM_EXTENSION);

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
* @param object The object to release
*/
template <typename T> void SafeRelease(T** object)
{
    if(*object)
    {
        (*object)->Release();
        *object = nullptr;
    }
}