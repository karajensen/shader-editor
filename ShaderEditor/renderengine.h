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
    * Renders the scene
    */
    virtual void Render() = 0;


};