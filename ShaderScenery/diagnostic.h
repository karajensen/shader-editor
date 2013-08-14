////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include "common.h"

/**
* Class for rendering the application shader/light diagnostics
*/
class Diagnostic : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit Diagnostic(EnginePtr engine);

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

    /**
    * Updates the light gui diagnostics with the currently selected light
    */
    void UpdateLightDiagnostics();

    /**
    * Updates the camera diagnostics with the currently selected camera
    */
    void UpdateCameraDiagnostics();

private:

    /**
    * Initialises the GUI objects for diagnostics
    */
    void InitialiseDiagnosticGUI();

    /**
    * Sets any custom ranges for the spin boxes
    */
    void SetSpinBoxRanges();

    bool m_render;       ///< Whether to allow rendering of diagnostics
    float m_textTimer;   ///< Timer for displaying diagnostics text
    bool m_runTimer;     ///< Start the timer for diagnostic text display
    int m_previousFPS;   ///< The frames for the previous second
    EnginePtr m_engine;  ///< Irrlicht engine

    IGUIStaticText* m_generalText;   ///< Generaral diagnostics text box


    //IGUIStaticText* m_cameraLabel;   ///< Diagnostic text for the camera type
    //IGUIStaticText* m_lightLabel;    ///< Selected light title
    //IGUIListBox* m_outTextureList;   ///< Listbox for possible output post textures
    //StaticTextVector m_bgrounds;     ///< Backgrounds for gui sections

    //SpinBoxVector m_shaderSpinBoxes;  ///< Spinboxes for shader options
    //SpinBoxVector m_lightSpinBoxes;   ///< Spinboxes for light options
    //SpinBoxVector m_cameraSpinBoxes;  ///< Spinboxes for camera options
};