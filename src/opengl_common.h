////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglc_ommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>

#include "glew/include/GL/glew.h"
#include "glew/include/GL/wglew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "render_data.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "texture_procedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "logger.h"

/**
* Callbacks for pre-rendering elements
*/
typedef std::function<void(const glm::mat4&, const Particle&)> PreRenderParticle;
typedef std::function<void(const glm::mat4&, int)> PreRenderMesh;

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
    case GL_INVALID_ENUM:
        Logger::LogError("OpenGL: Invalid Enum");
        return true;
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