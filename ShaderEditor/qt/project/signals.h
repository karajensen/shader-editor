////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - signals.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>

/**
* Holds functions called when a tweakable value emits a signal
*/
struct SignalCallbacks
{
    std::function<void(float)> SetLightPositionX;   ///< Sets the selected light position
    std::function<void(float)> SetLightPositionY;   ///< Sets the selected light position
    std::function<void(float)> SetLightPositionZ;   ///< Sets the selected light position
    std::function<void(float)> SetLightAttX;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightAttY;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightAttZ;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightDiffuseR;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightDiffuseG;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightDiffuseB;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightSpecularR;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularG;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularB;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularity; ///< Sets the selected light specular size
    std::function<void(float)> SetMeshSpecularity;  ///< Sets the selected mesh specular size
};