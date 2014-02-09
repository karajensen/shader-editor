////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"

/**
* OpenGL Graphics engine
*/
class OpenglEngine : public RenderEngine
{
public:

    /**
    * Constructor
    */
    OpenglEngine();

    /**
    * Destructor
    */
    ~OpenglEngine();

    /**
    * Sets up the graphics engine for rendering
    * @param hwnd the handle to the window
    * @return whether initialization succeeded
    */
    virtual bool Initialize(HWND hwnd) override;

    /**
    * Renders the scene
    */
    virtual void Render() override;

private:

    HGLRC m_hrc;        ///< Rendering context  
    HDC m_hdc;          ///< Device context  
    HWND m_hwnd;        ///< Window identifier  
};                     