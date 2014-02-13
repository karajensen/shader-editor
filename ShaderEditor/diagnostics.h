////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostics.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>

struct CTwBar;

/**
* Holds tweak bar and scene diagnostics
*/
class Diagnostics
{
public:

    /**
    * Constructor
    */
    Diagnostics();

    /**
    * Destructor
    */
    ~Diagnostics();

    /**
    * Removes the tweak bar
    */
    void RemoveTweakBar();

    /**
    * Initialises the tweak bar
    * @param opengl Whether to use openGl for rendering or not
    */
    void InitializeTweakBar(bool opengl);

    /**
    * Renders all diagnostics
    */
    void Render();

private:

    bool m_showTweakbar; ///< Whether the application tweak bar is visible
    CTwBar* m_tweakbar;  ///< Diagnostic tweak bar
};