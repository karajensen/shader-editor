////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <array>
#include "colour.h"

/**
* Holds data for post processing the final image
*/
struct PostProcessing
{
    /**
    * Post shader textures
    */
    enum Texture
    {
        SCENE,
        NORMAL,
        BLUR
    };

    /**
    * Post Shader maps
    */
    enum Map
    {
        FINAL_MAP,
        SCENE_MAP,
        NORMAL_MAP,
        DEPTH_MAP,
        GLOW_MAP,
        BLUR_SCENE_MAP,
        BLUR_GLOW_MAP,
        DOF_MAP,
        FOG_MAP,
        LIGHT_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing();

    /**
    * Normalises the blur weights
    */
    void NormaliseWeights();

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(Map map);

    /**
    * @param map the map type to convert
    * @return the string name of the map type
    */
    static std::string GetMapName(Map map);

    float dofDistance = 0.0f;            ///< Distance the depth of field starts
    float dofFade = 0.0f;                ///< How quick depth of field fades to the scene
    float glowAmount = 0.0f;             ///< The overall glow multiplier
    float contrast = 0.0f;               ///< Contrast controller of the final scene
    float saturation = 0.0f;             ///< Saturation controller of the final scene
    float blurStep = 0.0f;               ///< Sampling step for blurring
    float blurAmount = 0.0f;             ///< Amount to blur the scene by
    float depthNear = 0.0f;              ///< Value where depth colour is min
    float depthFar = 0.0f;               ///< Value where depth colour is max
    float fogDistance = 0.0f;            ///< Distance the fog starts
    float fogFade = 0.0f;                ///< How quick fog fades to the scene
    Colour fogColour;                    ///< Colour for the fog
    Colour minimumColour;                ///< Colour ranges for RGB
    Colour maximumColour;                ///< Colour ranges for RGB
    std::array<float, MAX_MAPS> masks;   ///< Visibility of post maps
    std::array<float, 5> weights;        ///< Normalised pixel weights for blurring
};