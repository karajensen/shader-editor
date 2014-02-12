////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include <memory>

struct OpenglData;

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
    * Begins rendering the scene
    */
    virtual void BeginRender() override;

    /**
    * Ends rendering the scene
    */
    virtual void EndRender() override;

private:

    std::unique_ptr<OpenglData> m_data; ///<  member data of opengl
};                     