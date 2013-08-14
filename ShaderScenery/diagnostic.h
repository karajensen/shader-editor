////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include <AntTweakBar.h>
#include "common.h"

class ShaderEditor;
class LightEditor;
class PostShader;

/**
* Class for rendering the application shader/light diagnostics
*/
class Diagnostic : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    * @param shader The editor to manipulate shaders
    * @parma postshader The post processing shader
    * @param light The editor to manipulate lights
    */
    Diagnostic(EnginePtr engine, 
        boost::shared_ptr<ShaderEditor> shader,
        boost::shared_ptr<PostShader> postshader,
        boost::shared_ptr<LightEditor> light);

    /**
    * Destructor
    */
    ~Diagnostic();

    /**
    * Renders the diagnostics
    * @param deltatime The time between frames
    */
    void Render(float deltatime);

    /**
    * Sets/shows the main diagnostic text for a specific period of time
    * @param text The text to show
    */
    void ShowDiagnosticText(const std::wstring& text);

    /**
    * Toggle whether to render diagnostics
    */
    void ToggleShowDiagnostics();

private:

    bool m_render;           ///< Whether to allow rendering of diagnostics
    float m_textTimer;       ///< Timer for displaying diagnostics text
    bool m_runTimer;         ///< Start the timer for diagnostic text display
    int m_previousFPS;       ///< The frames for the previous second
    EnginePtr m_engine;      ///< Irrlicht engine
    TwBar* m_tweakbar;       ///< Tweak bar for editing the application
    IGUIStaticText* m_text;  ///< Generaral diagnostics text box

    boost::shared_ptr<PostShader> m_postshader; ///< The post processing shader
    boost::shared_ptr<ShaderEditor> m_shader;   ///< Editor to manipulate shaders
    boost::shared_ptr<LightEditor> m_light;     ///< Editor to manipulate lights
};