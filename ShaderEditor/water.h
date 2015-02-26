////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "mesh.h"

/**
* Data for a water wave
*/
struct Wave
{
    float frequency = 0.0f;   ///< Frequency of the wave
    float amplitude = 0.0f;   ///< Amplitude of the wave
    float phase = 0.0f;       ///< Phase of the wave
    float directionX = 0.0f;  ///< Direction the wave moves
    float directionZ = 0.0f;  ///< Direction the wave moves
};

/**
* Data for a water-shaded mesh
*/
struct Water : public Mesh
{
    Float2 bumpVelocity;             ///< Velocity of the bump movement
    float speed = 0.0f;              ///< Overally speed of the wave
    Float2 uvScale;                  ///< Scale of the water texture
    Float3 fresnal;                  ///< The fresnal scale, bias and power
    Colour shallowColour;            ///< Colour intensity of the shallow water
    Colour deepColour;               ///< Colour intensity of the deep water
    Colour reflectionTint;           ///< Colour of the reflection tint
    float reflection = 0.0f;         ///< Intensity of the reflections

    static const int MAX_WAVES = 2;  ///< Currently supported maximum waves per water
    std::vector<Wave> waves;         ///< Wave infomation
};