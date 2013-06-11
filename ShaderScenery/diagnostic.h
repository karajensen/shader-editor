/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 
* Diagnostic static drawing class
*****************************************************************/
#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include "common.h"

class SpinBox;

class Diagnostic : boost::noncopyable
{
public:

    /**
    * @return the pointer to the diagnostics object
    */
    typedef boost::shared_ptr<Diagnostic> DiagPtr;
    static const DiagPtr& Get() { return sm_diag; }

    /**
    * Initialise the use of diagnostics
    */
    static void Initialise();

    /**
    * Renders the diagnostics
    * @param deltatime
    */
    void Render(float deltatime);

    /**
    * Sets/shows the main diagnostic text for a specific period of time
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
    * Constructor
    */
    Diagnostic();

    /**
    * Initialises the GUI objects for diagnostics
    */
    void InitialiseDiagnosticGUI();

    /**
    * Sets any custom ranges for the spin boxes
    */
    void SetSpinBoxRanges();

    typedef boost::shared_ptr<SpinBox> SpinBoxPtr;
    typedef std::vector<SpinBoxPtr> SpinBoxVector;
    typedef std::vector<IGUIStaticText*> StaticTextVector;

    static DiagPtr sm_diag;   ///< Diagnostic singleton pointer
    bool m_render;            ///< Whether to allow rendering of diagnostics
    float m_textTimer;        ///< Timer for displaying diagnostics text
    bool m_runTimer;          ///< Start the timer for diagnostic text display
    int m_previousFPS;        ///< The frames for the previous second

    IGUIStaticText* m_cameraLabel;   ///< Diagnostic text for the camera type
    IGUIStaticText* m_generalText;   ///< Generaral diagnostics text box
    IGUIStaticText* m_lightLabel;    ///< Selected light title
    IGUIListBox* m_outTextureList;   ///< Listbox for possible output post textures
    StaticTextVector m_bgrounds;     ///< Backgrounds for gui sections

    SpinBoxVector m_shaderSpinBoxes;  ///< Spinboxes for shader options
    SpinBoxVector m_lightSpinBoxes;   ///< Spinboxes for light options
    SpinBoxVector m_cameraSpinBoxes;  ///< Spinboxes for camera options
};

class SpinBox
{
public:

    typedef std::function<void(float)> ValueFn;

    /**
    * Constructor.
    * @param Function to call during update
    * @param the spin box dimensions
    * @param the text dimensions
    * @param the text to display
    */
    SpinBox(ValueFn valueFn, rect<s32> spinRect, rect<s32> textRect, const stringw& text);

    /**
    * Set the range of the spinbox
    * @param the min/maximum values the spinbox can have
    * @param stepping size
    * @param the number of decimal places
    */
    void SetRange(float min, float max, float stepSize, int precision);

    /**
    * Explicitly set the value of the spinbox
    * @param the value to set
    */
    void SetValue(float value);

    /**
    * Updates the check box by calling the given function
    */
    void Update();

private:

    ValueFn m_updateValueFn;  ///< Function to call during update
    IGUIStaticText* m_text;   ///< Text for the spin box
    IGUISpinBox* m_spinbox;   ///< Internal spin box
};