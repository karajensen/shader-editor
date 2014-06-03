////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "opengl/include/glew.h"
#include "opengl/include/wglew.h"
#include "opengl/include/GL.h"
#include "opengl/include/GLU.h"
#include "opengl/glm/glm.hpp"
#include "opengl/glm/gtc/matrix_transform.hpp"

const std::string POST_VERT("post" + GLSL_VERTEX);
const std::string POST_FRAG("post" + GLSL_FRAGMENT);
const std::string POST_VERT_FX(SHADER_PATH + POST_VERT + SHADER_EXTENSION);
const std::string POST_VERT_ASM(GENERATED_PATH + POST_VERT + ASM_EXTENSION);
const std::string POST_FRAG_FX(SHADER_PATH + POST_FRAG + SHADER_EXTENSION);
const std::string POST_FRAG_ASM(GENERATED_PATH + POST_FRAG + ASM_EXTENSION);

const std::string NORM_VERT("normal" + GLSL_VERTEX);
const std::string NORM_FRAG("normal" + GLSL_FRAGMENT);
const std::string NORM_VERT_FX(SHADER_PATH + NORM_VERT + SHADER_EXTENSION);
const std::string NORM_VERT_ASM(GENERATED_PATH + NORM_VERT + ASM_EXTENSION);
const std::string NORM_FRAG_FX(SHADER_PATH + NORM_FRAG + SHADER_EXTENSION);
const std::string NORM_FRAG_ASM(GENERATED_PATH + NORM_FRAG + ASM_EXTENSION);

/**
* OpenGL call checking
* @return whether the last call to OpenGL has failed
* @note requires an OpenGL context to be created
*/
inline bool HasCallFailed()
{
    switch(glGetError())
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_VALUE:
        Logger::LogError("OpenGL: Invalid Value");
        return true;
    case GL_INVALID_OPERATION:
        Logger::LogError("OpenGL: Invalid Operation");
        return true;
    default:
        Logger::LogError("OpenGL: Unknown Error");
        return true;
    }
}

/**
* Determines the unique ID of the texture slot
* @param slot The slot in the shader the texture will fill
* @return the unique ID of the texture
*/
inline unsigned int GetTexture(int slot)
{
    switch(slot)
    {
    case 0:
        return GL_TEXTURE0;
    case 1:
        return GL_TEXTURE1;
    case 2:
        return GL_TEXTURE2;
    case 3:
        return GL_TEXTURE3;
    case 4:
        return GL_TEXTURE4;
    case 5:
        return GL_TEXTURE5;
    case 6:
        return GL_TEXTURE6;
    case 7:
        return GL_TEXTURE7;
    case 8:
        return GL_TEXTURE8;
    default:
        Logger::LogError("Unknown texture slot");
        return 0;
    }
}