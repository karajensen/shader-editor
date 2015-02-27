////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "renderdata.h"
#include "opengl/include/glew.h"
#include "opengl/include/wglew.h"
#include "opengl/include/GL.h"
#include "opengl/include/GLU.h"
#include "opengl/glm/glm.hpp"
#include "opengl/glm/gtc/matrix_transform.hpp"

/**
* Callbacks for pre-rendering elements
*/
typedef std::function<void(const glm::mat4&, const Particle&)> PreRenderParticle;
typedef std::function<void(const glm::mat4&, const Colour&)> PreRenderMesh;

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