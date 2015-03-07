////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <vector>

/**
* Tweakable mesh values
*/
enum MeshAttribute
{
    MESH_AMBIENCE,
    MESH_BUMP,
    MESH_OVERLAY,
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
    LIGHT_DIFFUSE_R,
    LIGHT_DIFFUSE_G,
    LIGHT_DIFFUSE_B,
    LIGHT_SPECULAR_R,
    LIGHT_SPECULAR_G,
    LIGHT_SPECULAR_B,
    LIGHT_ACTIVE,
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
    EMITTER_LIFETIME,
    EMITTER_LIFEFADE,
    EMITTER_WIDTH,
    EMITTER_LENGTH,
    EMITTER_POS_X,
    EMITTER_POS_Y,
    EMITTER_POS_Z,
    EMITTER_DIR_X,
    EMITTER_DIR_Y,
    EMITTER_DIR_Z,
    EMITTER_TINT_R,
    EMITTER_TINT_G,
    EMITTER_TINT_B,
    EMITTER_MIN_SPEED,
    EMITTER_MAX_SPEED,
    EMITTER_MIN_FREQ,
    EMITTER_MAX_FREQ,
    EMITTER_MIN_AMP,
    EMITTER_MAX_AMP,
    EMITTER_MIN_SIZE,
    EMITTER_MAX_SIZE,
    EMITTER_MIN_WAVE,
    EMITTER_MAX_WAVE,
    EMITTER_ATTRIBUTES
};

/**
* Attributes for post processing
*/
enum PostAttribute
{
    POST_CONTRAST,
    POST_SATURATION,
    POST_BLUR_STEP,
    POST_DOF_START,
    POST_DOF_FADE,
    POST_DEPTH_NEAR,
    POST_DEPTH_FAR,
    POST_BLOOM_AMOUNT,
    POST_BLOOM_START,
    POST_BLOOM_FADE,
    POST_FOG_START,
    POST_FOG_FADE,
    POST_FOG_RED,
    POST_FOG_GREEN,
    POST_FOG_BLUE,
    POST_MIN_RED,
    POST_MIN_GREEN,
    POST_MIN_BLUE,
    POST_MAX_RED,
    POST_MAX_GREEN,
    POST_MAX_BLUE,
    POST_ATTRIBUTES
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
    FloatSignal SetBlurStep;       ///< Sets the amount of step between samples for blurring
    FloatSignal SetBloomIntensity; ///< Sets the amount of bloom on the scene
    FloatSignal SetBloomStart;     ///< Sets the threshold when the bloom will start
    FloatSignal SetContrast;       ///< Sets the contrast of the final scene
    FloatSignal SetSaturation;     ///< Sets the saturation of the final scene
    FloatSignal SetParticleAmount; ///< Sets the amount of particles the emitter spawns
    FloatSignals SetCamera;        ///< Sets the camera tweakable attributes
    FloatSignals SetLight;         ///< Sets the light tweakable attributes
    FloatSignals SetPost;          ///< Sets the post tweakable attributes
    FloatSignals SetMesh;          ///< Sets the mesh tweakable attributes
    FloatSignals SetWater;         ///< Sets the water tweakable attributes
    FloatSignals SetWave;          ///< Sets the water wave tweakable attributes
    FloatSignals SetEmitter;       ///< Sets the emitter tweakable attributes
    FloatSignal SetSelectedWave;   ///< Sets the selected wave to display
    IntSignal SetSelectedEngine;   ///< Sets the selected engine to render
    IntSignal SetSelectedLight;    ///< Sets the selected light to display
    IntSignal SetSelectedMesh;     ///< Sets the selected mesh to display
    IntSignal SetSelectedWater;    ///< Sets the selected water to display
    IntSignal SetSelectedShader;   ///< Sets the selected shader to edit
    IntSignal SetSelectedEmitter;  ///< Sets the selected emitter to edit
    IntSignal SetPostMap;          ///< Sets the selected post map to display
    VoidSignal LightDiagnostics;   ///< Sends a request to toggle the light diagnostics
    VoidSignal ReloadScene;        ///< Sends a request to reload the scene
    VoidSignal SaveScene;          ///< Sends a request to save the scene to xml
    VoidSignal PauseEmission;      ///< Sends a request to pause the selected Emitter
    VoidSignal RenderLightsOnly;   ///< Sends a request to render only the lights
    StrSignal CompileShader;       ///< Sends a compile request

    /**
    * Constructor
    */
    SignalCallbacks() :
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
        LightDiagnostics(nullptr),
        SaveScene(nullptr),
        PauseEmission(nullptr),
        RenderLightsOnly(nullptr),
        CompileShader(nullptr)
    {
        SetLight.resize(LIGHT_ATTRIBUTES);
        SetLight.assign(LIGHT_ATTRIBUTES, nullptr);

        SetCamera.resize(CAMERA_ATTRIBUTES);
        SetCamera.assign(CAMERA_ATTRIBUTES, nullptr);

        SetMesh.resize(MESH_ATTRIBUTES);
        SetMesh.assign(MESH_ATTRIBUTES, nullptr);

        SetWater.resize(WATER_ATTRIBUTES);
        SetWater.assign(WATER_ATTRIBUTES, nullptr);

        SetWave.resize(WAVE_ATTRIBUTES);
        SetWave.assign(WAVE_ATTRIBUTES, nullptr);

        SetEmitter.resize(EMITTER_ATTRIBUTES);
        SetEmitter.assign(EMITTER_ATTRIBUTES, nullptr);

        SetPost.resize(POST_ATTRIBUTES);
        SetPost.assign(POST_ATTRIBUTES, nullptr);
    }
};
