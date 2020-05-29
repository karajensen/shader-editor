////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - render_data.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <float3.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int MULTISAMPLING_COUNT = 4;
constexpr int MAX_ANISOTROPY = 16;
constexpr float FRUSTRUM_NEAR = 1.0f;
constexpr float FRUSTRUM_FAR = 1500.0f; // Minimum value for skybox
constexpr float FRUSTRUM_BOUNDS_NEAR = 0.0f;
constexpr float FRUSTRUM_BOUNDS_FAR = 1500.0f;
constexpr float FIELD_OF_VIEW = 60.0f;
constexpr float RATIO = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);
const std::string ASSETS_PATH(".//Assets//");

constexpr int MAX_TARGETS = 2;      ///< Maximum used multiple textures to a target
constexpr int SCENE_TEXTURES = 2;   ///< Number of textures attached to the scene target
constexpr int EFFECTS_TEXTURES = 1; ///< Number of textures attached to the effects target
constexpr int BLUR_TEXTURES = 1;    ///< Number of textures attached to the effects target
constexpr int SCENE_ID = 0;         ///< ID of the texture displaying the scene
constexpr int DEPTH_ID = 1;         ///< ID of the texture displaying the depth information
constexpr int BLUR_ID = 0;          ///< ID of the texture displaying the blur

/**
* Supported Render Engines
*/
namespace RenderingEngine
{
    enum Engine
    {
        OpenGL,
        DirectX,
        Max
    };
}

/**
* Index for special textures
*/
namespace TextureIndex
{
    enum Index
    {
        BlankTexture,
        Max
    };
}

/**
* Index for special shaders
*/
namespace ShaderIndex
{
    enum Index
    {
        Post,
        Pre,
        BlurHorizontal,
        BlurVertical,
        Water,
        Particle,
        Diagnostic,
        Shadow,
        Max
    };
}

/**
* Type of texture to send to the mesh shader
* Ordering must match usage in shader body
*/
namespace TextureSlot
{
    enum Slot
    {
        Diffuse,
        Normal,
        Specular,
        Environment,
        Caustics,
        Max
    };
}

/**
* Circle bounding area data
*/
struct BoundingArea
{
    Float3 center;
    float radius = 0.0f;
};