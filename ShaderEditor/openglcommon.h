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

/**
* OpenGL call checking
* @return whether the last call to OpenGL has failed
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