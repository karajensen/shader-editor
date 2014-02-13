////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostics.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "diagnostics.h"
#include "common.h"
#include "AntTweakBar.h"

Diagnostics::Diagnostics() :
    m_tweakbar(nullptr),
    m_showTweakbar(true)
{
}

Diagnostics::~Diagnostics()
{
    RemoveTweakBar();
}

void Diagnostics::Render()
{
    if(m_showTweakbar)
    {
        TwDraw();
    }
}

void Diagnostics::InitializeTweakBar(bool opengl)
{
    RemoveTweakBar();

    if(opengl)
    {
        TwInit(TW_OPENGL_CORE, nullptr);
    }
    else
    {
        //note requires device pointer
        TwInit(TW_DIRECT3D11, nullptr);
    }

    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string barname = "ShaderEditor";
    m_tweakbar = TwNewBar(barname.c_str());

    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Shader Editor' " 
        << "position='" << border << " " << border << "' "
        << "size='200 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=false "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());
}

void Diagnostics::RemoveTweakBar()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}