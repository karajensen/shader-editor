/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 
* Diagnostic static drawing class
*****************************************************************/
#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include "common.h"

class CheckBox;

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

    static DiagPtr sm_diag;   ///< Diagnostic singleton pointer
    bool m_render;            ///< Whether to allow rendering of diagnostics
    SColor m_bgroundCol;      ///< Colour for diag text box background
    SColor m_clearCol;        ///< Colour for a clean background
    float m_textTimer;        ///< Timer for displaying diagnostics text
    bool m_runTimer;          ///< Start the timer for diagnostic text display
    int m_previousFPS;        ///< The frames for the previous second
    IGUIStaticText* m_text;   ///< Diagnostics text box
    IGUIStaticText* m_light;  ///< Diagnostics text box for lights

    typedef boost::shared_ptr<CheckBox> CheckBoxPtr;

    IGUIListBox* m_outTextureList;          ///< Listbox for possible output post textures
    std::vector<CheckBoxPtr> m_checkBoxes;  ///< Checkboxes for toggled shader options
    IGUIStaticText* m_componentBground;     ///< Text background for component check boxes

};

class CheckBox
{
public:

    typedef std::function<void(bool)> CheckFn;

    /**
    * Constructor.
    * @param Function to call during update
    * @param the text box dimensions
    * @param the text to display
    */
    CheckBox(CheckFn checkFn, rect<s32> textBox, const stringw& text);

    /**
    * Updates the check box by calling the given function
    */
    void Update();

private:

    CheckFn m_onCheckFn;      ///< Function to call during update
    IGUICheckBox* m_checkbox; ///< Internal check box
    unsigned int m_component; ///< Component index to affect
};