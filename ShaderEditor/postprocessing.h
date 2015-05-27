////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include <array>
#include "colour.h"

struct Cache;

/**
* Holds data for post processing the final image
*/
class PostProcessing
{
public:

    /**
    * Post Shader maps
    */
    enum Map
    {
        FINAL_MAP,
        SCENE_MAP,
        DEPTH_MAP,
        BLUR_MAP,
        DOF_MAP,
        FOG_MAP,
        BLOOM_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing();

    /**
    * Writes to the data in the cache from the post data
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache into the post data
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(Map map);

    /**
    * @param map the map type to convert
    * @return The string name of the map type
    */
    static std::string GetMapName(Map map);

    /**
    * @return Distance the depth of field starts
    */
    const float& DOFStart() const;

    /**
    * @return How quick depth of field fades to the scene
    */
    const float& DOFFade() const;

    /**
    * @return The blur weights for the surrounding pixels
    */
    const std::vector<float>& GetBlurWeights() const;

    /**
    * @return Contrast controller of the final scene
    */
    const float& Contrast() const;

    /**
    * @return Saturation controller of the final scene
    */
    const float& Saturation() const;

    /**
    * @return Sampling step for blurring
    */
    const float& BlurStep() const;

    /**
    * @return Value where depth colour is min
    */
    const float& DepthNear() const;

    /**
    * @return Value where depth colour is max
    */
    const float& DepthFar() const;

    /**
    * @return Distance the fog starts
    */
    const float& FogStart() const;

    /**
    * @return How quick fog fades to the scene
    */
    const float& FogFade() const;

    /**
    * @return Colour for the fog
    */
    const Colour& FogColour() const;

    /**
    * @return Minimum Colour ranges for RGB
    */
    const Colour& MinColour() const;

    /**
    * @return Maximum Colour ranges for RGB
    */
    const Colour& MaxColour() const;

    /**
    * @return The mask value for the map
    */
    const float& Mask(PostProcessing::Map map) const;

    /**
    * @return The itensity of the bloom
    */
    const float& BloomIntensity() const;

    /**
    * @return The threshold when the bloom starts
    */
    const float& BloomStart() const;

    /**
    * @return The fadeoff of the bloom
    */
    const float& BloomFade() const;

    /**
    * Toggles whether to render with diffuse textures
    */
    void ToggleDiffuseTextures();

    /**
    * @return Whether to render with diffuse textures
    */
    bool UseDiffuseTextures() const;

private:

    /**
    * Normalises the blur weights
    */
    void NormaliseWeights();

    bool m_useDiffuseTextures = true;     ///< Whether to render diffuse textures;
    float m_dofStart = 0.0f;              ///< Distance the depth of field starts
    float m_dofFade = 0.0f;               ///< How quick depth of field fades to the scene
    float m_bloomStart = 0.0f;            ///< The threshold when the bloom starts
    float m_bloomIntensity = 0.0f;        ///< Intensity of the bloom
    float m_bloomFade = 0.0f;             ///< The amount of fade off
    float m_contrast = 0.0f;              ///< Contrast controller of the final scene
    float m_saturation = 0.0f;            ///< Saturation controller of the final scene
    float m_blurStep = 0.0f;              ///< Sampling step for blurring
    float m_depthNear = 0.0f;             ///< Value where depth colour is min
    float m_depthFar = 0.0f;              ///< Value where depth colour is max
    float m_fogStart = 0.0f;              ///< Distance the fog starts
    float m_fogFade = 0.0f;               ///< How quick fog fades to the scene
    Colour m_fogColour;                   ///< Colour for the fog
    Colour m_minimumColour;               ///< Minimum Colour ranges for RGB
    Colour m_maximumColour;               ///< Maximum Colour ranges for RGB
    std::array<float, MAX_MAPS> m_masks;  ///< Visibility of post maps
    std::vector<float> m_weights;         ///< Normalised pixel weights for blurring
};