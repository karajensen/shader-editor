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
    * @param the text for light diagnosticss
    */
    void Render(float deltatime, const stringw& lighttext);

    /**
    * Sets/shows the main diagnostic text for a specific period of time
    */
    void ShowDiagnosticText(const std::wstring& text);

    /**
    * Toggle whether to render diagnostics
    */
    void ToggleShowDiagnostics();

private:

    /**
    * Constructor
    */
    Diagnostic();

    /**
    * Initialises all shader diagnostics
    */
    void InitialiseShaderDiagnostics();

    /**
    * Initialises all light diagnostics
    */
    void InitialiseLightDiagnostics();


    static DiagPtr sm_diag;   ///< Diagnostic singleton pointer
    bool m_render;            ///< Whether to allow rendering of diagnostics
    float m_textTimer;        ///< Timer for displaying diagnostics text
    bool m_runTimer;          ///< Start the timer for diagnostic text display
    int m_previousFPS;        ///< The frames for the previous second
    IGUIStaticText* m_text;   ///< Diagnostics text box
    IGUIStaticText* m_light;  ///< Diagnostics text box for lights

    typedef boost::shared_ptr<SpinBox> SpinBoxPtr;

    IGUIListBox* m_outTextureList;           ///< Listbox for possible output post textures
    std::vector<SpinBoxPtr> m_spinBoxes;     ///< Checkboxes for toggled shader options
    std::vector<IGUIStaticText*> m_bgrounds; ///< Backgrounds for gui sections
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
    * Updates the check box by calling the given function
    */
    void Update();

private:

    ValueFn m_updateValueFn;  ///< Function to call during update
    IGUIStaticText* m_text;   ///< Text for the spin box
    IGUISpinBox* m_spinbox;   ///< Internal spin box
};