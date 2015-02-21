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
    WATER_BUMP_VELOCITY_X,
    WATER_BUMP_VELOCITY_Y,
    WATER_SPEED,
    WATER_SCALE_U,
    WATER_SCALE_V,
    WATER_FRESNAL_SCALE,
    WATER_FRESNAL_BIAS,
    WATER_FRESNAL_POWER,
    WATER_SHALLOW_R,
    WATER_SHALLOW_G,
    WATER_SHALLOW_B,
    WATER_SHALLOW_A,
    WATER_DEEP_R,
    WATER_DEEP_G,
    WATER_DEEP_B,
    WATER_DEEP_A,
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
    WAVE_PHASE,
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
* Tweakable camera values
*/
enum CameraAttribute
{
    CAMERA_POSITION_X,
    CAMERA_POSITION_Y,
    CAMERA_POSITION_Z,
    CAMERA_PITCH,
    CAMERA_YAW,
    CAMERA_ROLL,
    CAMERA_ATTRIBUTES
};

/**
* Attributes for the emitter
*/
enum EmitterAttribute
{
    EMITTER_WIDTH,
    EMITTER_LENGTH,
    EMITTER_POS_X,
    EMITTER_POS_Y,
    EMITTER_POS_Z,
    EMITTER_DIR_X,
    EMITTER_DIR_Y,
    EMITTER_DIR_Z,
    EMITTER_ATTRIBUTES
};

/**
* Attributes for the particle
*/
enum ParticleAttribute
{
    PARTICLE_MIN_SPEED,
    PARTICLE_MAX_SPEED,
    PARTICLE_LIFETIME,
    PARTICLE_MIN_SIZE,
    PARTICLE_MAX_SIZE,
    PARTICLE_TINT_R,
    PARTICLE_TINT_G,
    PARTICLE_TINT_B,
    PARTICLE_ATTRIBUTES
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
    FloatSignal SetDepthNear;      ///< Sets the depth near value
    FloatSignal SetDepthFar;       ///< Sets the depth far value
    FloatSignal SetDOFDistance;    ///< Sets when depth of field kicks in
    FloatSignal SetDOFFade;        ///< Sets how quickly depth of field fades to the scene
    FloatSignal SetBlurAmount;     ///< Sets the amount of blurring on the scene
    FloatSignal SetBlurStep;       ///< Sets the amount of step between samples for blurring
    FloatSignal SetGlowAmount;     ///< Sets the amount of glow on the scene
    FloatSignal SetContrast;       ///< Sets the contrast of the final scene
    FloatSignal SetSaturation;     ///< Sets the saturation of the final scene
    FloatSignal SetParticleAmount; ///< Sets the amount of particles the emitter spawns
    FloatSignals SetCamera;        ///< Sets the camera tweakable attributes
    FloatSignals SetLight;         ///< Sets the light tweakable attributes
    FloatSignals SetFog;           ///< Sets the fog tweakable attributes
    FloatSignals SetMesh;          ///< Sets the mesh tweakable attributes
    FloatSignals SetWater;         ///< Sets the water tweakable attributes
    FloatSignals SetWave;          ///< Sets the water wave tweakable attributes
    FloatSignals SetMinColour;     ///< Sets the minimum post colour range
    FloatSignals SetMaxColour;     ///< Sets the maximum post colour range
    FloatSignals SetEmitter;       ///< Sets the emitter tweakable attributes
    FloatSignals SetParticles;     ///< Sets the particles tweakable attributes
    FloatSignal SetSelectedWave;   ///< Sets the selected wave to display
    IntSignal SetSelectedEngine;   ///< Sets the selected engine to render
    IntSignal SetSelectedLight;    ///< Sets the selected light to display
    IntSignal SetSelectedMesh;     ///< Sets the selected mesh to display
    IntSignal SetSelectedWater;    ///< Sets the selected water to display
    IntSignal SetSelectedShader;   ///< Sets the selected shader to edit
    IntSignal SetSelectedEmitter;  ///< Sets the selected emitter to edit
    IntSignal SetPostMap;          ///< Sets the selected post map to display
    VoidSignal ReloadScene;        ///< Sends a request to reload the scene
    VoidSignal SaveScene;          ///< Sends a request to save the scene to xml
    VoidSignal PauseEmission;      ///< Sends a request to pause the selected Emitter
    VoidSignal SavePost;           ///< Sends a request to save post processing to xml
    StrSignal CompileShader;       ///< Sends a compile request

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
        SetParticleAmount(nullptr),
        SetSelectedEngine(nullptr),
        SetSelectedLight(nullptr),
        SetSelectedMesh(nullptr),
        SetSelectedWater(nullptr),
        SetSelectedShader(nullptr),
        SetSelectedEmitter(nullptr),
        SetSelectedWave(nullptr),
        SetPostMap(nullptr),
        ReloadScene(nullptr),
        SaveScene(nullptr),
        PauseEmission(nullptr),
        SavePost(nullptr),
        CompileShader(nullptr)
    {
        SetLight.resize(LIGHT_ATTRIBUTES);
        SetLight.assign(LIGHT_ATTRIBUTES, nullptr);

        SetCamera.resize(CAMERA_ATTRIBUTES);
        SetCamera.assign(CAMERA_ATTRIBUTES, nullptr);

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

        SetWave.resize(WAVE_ATTRIBUTES);
        SetWave.assign(WAVE_ATTRIBUTES, nullptr);

        SetEmitter.resize(EMITTER_ATTRIBUTES);
        SetEmitter.assign(EMITTER_ATTRIBUTES, nullptr);

        SetParticles.resize(PARTICLE_ATTRIBUTES);
        SetParticles.assign(PARTICLE_ATTRIBUTES, nullptr);
    }
};
