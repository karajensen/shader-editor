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
    std::function<void(float)> SetMinRed;           ///< Sets the minimum value for red in post
    std::function<void(float)> SetMinGreen;         ///< Sets the minimum value for green in post
    std::function<void(float)> SetMinBlue;          ///< Sets the minimum value for blue in post
    std::function<void(float)> SetMinColour;        ///< Sets the minimum value for colour in post
    std::function<void(float)> SetMaxRed;           ///< Sets the maximum value for red in post
    std::function<void(float)> SetMaxGreen;         ///< Sets the maximum value for green in post
    std::function<void(float)> SetMaxBlue;          ///< Sets the maximum value for blue in post
    std::function<void(float)> SetMaxColour;        ///< Sets the maximum value for colour in post
    std::function<void(int)> SetSelectedEngine;     ///< Sets the selected engine to render
    std::function<void(int)> SetSelectedLight;      ///< Sets the selected light to display
    std::function<void(int)> SetSelectedMesh;       ///< Sets the selected mesh to display
    std::function<void(int)> SetSelectedShader;     ///< Sets the selected shader to edit
    std::function<void(int)> SetPostTexture;        ///< Sets the selected post texture to display
};