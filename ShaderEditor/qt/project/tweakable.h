////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <vector>

/**
* Tweakable colour values
*/
enum ColourAttribute
{
    RED,
    GREEN,
    BLUE,
    COLOUR_ATTRIBUTES
};

/**
* Tweakable fog values
*/
enum FogAttribute
{
    FOG_DISTANCE,
    FOG_FADE,
    FOG_RED,
    FOG_GREEN,
    FOG_BLUE,
    FOG_ATTRIBUTES
};

/**
* Tweakable mesh values
*/
enum MeshAttribute
{
    MESH_AMBIENCE,
    MESH_GLOW,
    MESH_BUMP,
    MESH_SPECULARITY,
    MESH_ATTRIBUTES
};

/**
* Tweakable water values
*/
enum WaterAttribute
{
    WATER_BUMP,
    WATER_BUMP_SPEED,
    WATER_SPEED,
    WATER_OFFSET_U,
    WATER_OFFSET_V,
    WATER_FRESNAL_FACTOR,
    WATER_SHALLOW_R,
    WATER_SHALLOW_G,
    WATER_SHALLOW_B,
    WATER_DEEP_R,
    WATER_DEEP_G,
    WATER_DEEP_B,
    WATER_REFLECTION_R,
    WATER_REFLECTION_G,
    WATER_REFLECTION_B,
    WATER_REFLECTION,
    WATER_ATTRIBUTES
};

/**
* Tweakable wave values
*/
enum WaveAttribute
{
    WAVE_FREQUENCY,
    WAVE_AMPLITUDE,
    WAVE_SPEED,
    WAVE_DIRECTION_X,
    WAVE_DIRECTION_Z,
    WAVE_ATTRIBUTES
};

/**
* Tweakable light values
*/
enum LightAttribute
{
    LIGHT_POSITION_X,
    LIGHT_POSITION_Y,
    LIGHT_POSITION_Z,
    LIGHT_ATTENUATION_X,
    LIGHT_ATTENUATION_Y,
    LIGHT_ATTENUATION_Z,
    LIGHT_SPECULARITY,
    LIGHT_SPECULAR_R,
    LIGHT_SPECULAR_G,
    LIGHT_SPECULAR_B,
    LIGHT_DIFFUSE_R,
    LIGHT_DIFFUSE_G,
    LIGHT_DIFFUSE_B,
    LIGHT_ATTRIBUTES
};

/**
* Supported callback types
*/
typedef std::function<void(float)> FloatSignal;
typedef std::function<void(int)> IntSignal;
typedef std::function<void(void)> VoidSignal;
typedef std::function<void(const std::string&)> StrSignal;
typedef std::vector<FloatSignal> FloatSignals;

/**
* Holds functions called when a tweakable value emits a signal
*/
struct SignalCallbacks
{
    FloatSignal SetDepthNear;     ///< Sets the depth near value
    FloatSignal SetDepthFar;      ///< Sets the depth far value
    FloatSignal SetDOFDistance;   ///< Sets when depth of field kicks in
    FloatSignal SetDOFFade;       ///< Sets how quickly depth of field fades to the scene
    FloatSignal SetBlurAmount;    ///< Sets the amount of blurring on the scene
    FloatSignal SetBlurStep;      ///< Sets the amount of step between samples for blurring
    FloatSignal SetGlowAmount;    ///< Sets the amount of glow on the scene
    FloatSignal SetContrast;      ///< Sets the contrast of the final scene
    FloatSignal SetSaturation;    ///< Sets the saturation of the final scene
    FloatSignals SetLight;        ///< Sets the light tweakable attributes
    FloatSignals SetFog;          ///< Sets the fog tweakable attributes
    FloatSignals SetMesh;         ///< Sets the mesh tweakable attributes
    FloatSignals SetWater;        ///< Sets the water tweakable attributes
    FloatSignals SetMinColour;    ///< Sets the minimum post colour range
    FloatSignals SetMaxColour;    ///< Sets the maximum post colour range
    IntSignal SetSelectedEngine;  ///< Sets the selected engine to render
    IntSignal SetSelectedLight;   ///< Sets the selected light to display
    IntSignal SetSelectedMesh;    ///< Sets the selected mesh to display
    IntSignal SetSelectedWater;   ///< Sets the selected water to display
    IntSignal SetSelectedShader;  ///< Sets the selected shader to edit
    IntSignal SetPostMap;         ///< Sets the selected post map to display
    VoidSignal SaveLights;        ///< Sends a request to save the lights to xml
    VoidSignal SaveMeshes;        ///< Sends a request to save the meshes to xml
    VoidSignal SavePost;          ///< Sends a request to save post processing to xml
    StrSignal CompileShader;      ///< Sends a compile request

    /**
    * Constructor
    */
    SignalCallbacks() :
        SetDepthNear(nullptr),
        SetDepthFar(nullptr),
        SetDOFDistance(nullptr),
        SetDOFFade(nullptr),
        SetBlurAmount(nullptr),
        SetBlurStep(nullptr),
        SetGlowAmount(nullptr),
        SetContrast(nullptr),
        SetSaturation(nullptr),
        SetSelectedEngine(nullptr),
        SetSelectedLight(nullptr),
        SetSelectedMesh(nullptr),
        SetSelectedWater(nullptr),
        SetSelectedShader(nullptr),
        SetPostMap(nullptr),
        SaveLights(nullptr),
        SaveMeshes(nullptr),
        SavePost(nullptr),
        CompileShader(nullptr)
    {
        SetLight.resize(LIGHT_ATTRIBUTES);
        SetLight.assign(LIGHT_ATTRIBUTES, nullptr);

        SetFog.resize(FOG_ATTRIBUTES);
        SetFog.assign(FOG_ATTRIBUTES, nullptr);

        SetMesh.resize(MESH_ATTRIBUTES);
        SetMesh.assign(MESH_ATTRIBUTES, nullptr);

        SetWater.resize(WATER_ATTRIBUTES);
        SetWater.assign(WATER_ATTRIBUTES, nullptr);

        SetMinColour.resize(COLOUR_ATTRIBUTES);
        SetMinColour.assign(COLOUR_ATTRIBUTES, nullptr);

        SetMaxColour.resize(COLOUR_ATTRIBUTES);
        SetMaxColour.assign(COLOUR_ATTRIBUTES, nullptr);
    }
};
