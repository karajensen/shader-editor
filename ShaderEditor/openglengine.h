////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include <Windows.h>

/**
* OpenGL Graphics engine
*/
class OpenglEngine : public RenderEngine
{
public:

    /**
    * Sets up the graphics engine for rendering
    */
    virtual void Initialize() override;

    /**
    * Renders the scene
    */
    virtual void Render() override;

private:

    HGLRC hrc;   ///< Rendering context  
    HDC hdc;     ///< Device context  
    HWND hwnd;   ///< Window identifier  
};