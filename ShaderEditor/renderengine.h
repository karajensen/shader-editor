////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>

/**
* Base graphics API interface
*/
class RenderEngine
{
public:

    /**
    * Sets up the graphics engine for rendering
    * @param hwnd the handle to the window
    * @return whether initialization succeeded
    */
    virtual bool Initialize(HWND hwnd) = 0;

    /**
    * Begins rendering the scene
    */
    virtual void BeginRender() = 0;

    /**
    * Ends rendering the scene
    */
    virtual void EndRender() = 0;


};