////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Base graphics API interface
*/
class RenderEngine
{
public:

    /**
    * Sets up the graphics engine for rendering
    */
    virtual void Initialize() = 0;

    /**
    * Renders the scene
    */
    virtual void Render() = 0;


};