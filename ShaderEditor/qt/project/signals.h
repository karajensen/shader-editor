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
    std::function<void(const std::string&)> CompileShader; ///< Sends a compile request
    std::function<void(float)> SetDepthNear;        ///< Sets the depth near value
    std::function<void(float)> SetDepthFar;         ///< Sets the depth far value
    std::function<void(float)> SetDOFDistance;      ///< Sets when depth of field kicks in
    std::function<void(float)> SetDOFFade;          ///< Sets how quickly depth of field fades to the scene
    std::function<void(float)> SetBlurAmount;       ///< Sets the amount of blurring on the scene
    std::function<void(float)> SetBlurStep;         ///< Sets the amount of step between samples for blurring
    std::function<void(float)> SetGlowAmount;       ///< Sets the amount of glow on the scene
    std::function<void(float)> SetContrast;         ///< Sets the contrast of the final scene
    std::function<void(float)> SetSaturation;       ///< Sets the saturation of the final scene
    std::function<void(float)> SetFogDistance;      ///< Sets when fog kicks in
    std::function<void(float)> SetFogFade;          ///< Sets how quickly fog fades to the scene
    std::function<void(float)> SetFogColourR;       ///< Sets the colour of the fog
    std::function<void(float)> SetFogColourG;       ///< Sets the colour of the fog
    std::function<void(float)> SetFogColourB;       ///< Sets the colour of the fog
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
    std::function<void(float)> SetMeshAmbience;     ///< Sets the selected mesh ambience
    std::function<void(float)> SetMeshBump;         ///< Sets the selected mesh bump saturation
    std::function<void(float)> SetMeshGlow;         ///< Sets the selected mesh glow intensity
    std::function<void(float)> SetMinRed;           ///< Sets the minimum value for red in post
    std::function<void(float)> SetMinGreen;         ///< Sets the minimum value for green in post
    std::function<void(float)> SetMinBlue;          ///< Sets the minimum value for blue in post
    std::function<void(float)> SetMaxRed;           ///< Sets the maximum value for red in post
    std::function<void(float)> SetMaxGreen;         ///< Sets the maximum value for green in post
    std::function<void(float)> SetMaxBlue;          ///< Sets the maximum value for blue in post
    std::function<void(int)> SetSelectedEngine;     ///< Sets the selected engine to render
    std::function<void(int)> SetSelectedLight;      ///< Sets the selected light to display
    std::function<void(int)> SetSelectedMesh;       ///< Sets the selected mesh to display
    std::function<void(int)> SetSelectedShader;     ///< Sets the selected shader to edit
    std::function<void(int)> SetPostMap;            ///< Sets the selected post map to display
    std::function<void(void)> SaveLights;           ///< Sends a request to save the lights to xml
    std::function<void(void)> SaveMeshes;           ///< Sends a request to save the meshes to xml
    std::function<void(void)> SavePost;             ///< Sends a request to save post processing to xml
};