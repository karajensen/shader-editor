////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MULTISAMPLING_COUNT = 4;
const int MAX_ANISOTROPY = 16;
const int RANDOM_TEXTURE_SIZE = 128;
const float FRUSTRUM_NEAR = 1.0f;
const float FRUSTRUM_FAR = 2000.0f;
const float FIELD_OF_VIEW = 60.0f;

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
* Number of textures attached to the render targets
* Scene Target 0: scene (rgb)
* Scene Target 1: normals (rgb) / depth (a)
* Effects Target 0: scene (rgb) / bloom (a)
* Effects Target 1: normals or ambience (rgb) / depth (a)
* Blur Target 0: scene (rgb) / bloom (a)
*/
const int EFFECTS_TEXTURES = 2;
const int SCENE_TEXTURES = 2;
const int BLUR_TEXTURES = 1;
const int SCENE_ID = 0;
const int NORMAL_ID = 1;
const int EFFECTS_ID = 1;
const int BLUR_ID = 0;

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
    RANDOM_TEXTURE_ID
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