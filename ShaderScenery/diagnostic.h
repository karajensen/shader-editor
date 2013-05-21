/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 
* Diagnostic static drawing class
*****************************************************************/
#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "common.h"

class Diagnostic : boost::noncopyable
{
public:

    /**
    * Easy access diagnostic colours
    */
    enum Colour
    {
        WHITE,
        RED,
        GREEN,
        BLUE
    };

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
    * Destructor
    */
    ~Diagnostic();

private:

    Diagnostic();

    static DiagPtr sm_diag; ///< Diagnostic singleton pointer

    bool m_render;           ///< Whether to allow rendering of diagnostics
    SColor m_bgroundCol;     ///< Colour for diag text box background
    SColor m_clearCol;       ///< Colour for a clean background
    float m_textTimer;       ///< Timer for displaying diagnostics text
    bool m_runTimer;         ///< Start the timer for diagnostic text display
    int m_previousFPS;       //< The frames for the previous second
    IGUIStaticText* m_text;  ///< Diagnostics text box
    IGUIStaticText* m_light; ///< Diagnostics text box for lights

};