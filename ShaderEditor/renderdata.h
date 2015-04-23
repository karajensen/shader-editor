////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MULTISAMPLING_COUNT = 4;
const int MAX_ANISOTROPY = 16;
const float FRUSTRUM_NEAR = 1.0f;
const float FRUSTRUM_FAR = 1500.0f; // Minimum value for skybox
const float FRUSTRUM_BOUNDS_NEAR = 0.0f;
const float FRUSTRUM_BOUNDS_FAR = 1000.0f;
const float FIELD_OF_VIEW = 60.0f;
const float RATIO = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);

const std::string SHADER_EXTENSION(".fx");
const std::string ASM_EXTENSION(".as");
const std::string GLSL_VERTEX("_glsl_vert");
const std::string GLSL_FRAGMENT("_glsl_frag");
const std::string HLSL_SHADER("_hlsl");
const std::string GLSL_FRAGMENT_EXTENSION(GLSL_FRAGMENT + SHADER_EXTENSION);   
const std::string GLSL_VERTEX_EXTENSION(GLSL_VERTEX + SHADER_EXTENSION);
const std::string HLSL_SHADER_EXTENSION(HLSL_SHADER + SHADER_EXTENSION);

const std::string ASSETS_PATH(".//Assets//");
const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
const std::string GENERATED_PATH(SHADER_PATH + "Generated//");

/**
* Data for the render targets and attached textures
*/
enum RenderTargets
{
    MAX_TARGETS = 2,      ///< Maximum used multiple textures to a target
    SCENE_TEXTURES = 2,   ///< Number of textures attached to the scene target
    EFFECTS_TEXTURES = 1, ///< Number of textures attached to the effects target
    BLUR_TEXTURES = 1,    ///< Number of textures attached to the effects target
    SCENE_ID = 0,         ///< ID of the texture displaying the scene
    DEPTH_ID = 1,         ///< ID of the texture displaying the depth information
    BLUR_ID = 0,          ///< ID of the texture displaying the blur
};                        

/**
* Available render engines
*/
enum RenderingEngine
{
    OPENGL,
    DIRECTX,
    MAX_ENGINES
};

/**
* Index for special textures
*/
enum TextureIndex
{
    BLANK_TEXTURE_ID,
    MAX_TEXTURES
};

/**
* Index for special shaders
*/
enum ShaderIndex
{
    POST_SHADER,
    PRE_SHADER,
    BLUR_HORIZONTAL_SHADER,
    BLUR_VERTICAL_SHADER,
    WATER_SHADER,
    PARTICLE_SHADER,
    DIAGNOSTIC_SHADER
};

/**
* Type of texture to send to the mesh shader
* Ordering must match usage in shader body
*/
enum TextureSlot
{
    SLOT_DIFFUSE,
    SLOT_NORMAL,
    SLOT_SPECULAR,
    SLOT_ENVIRONMENT,
    SLOT_CAUSTICS,
    MAX_SLOTS
};

/**
* Bounding area data
*/
struct BoundingArea
{
    Float3 center;
    float radius = 0.0f;
};