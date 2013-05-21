#include "diagnostic.h"
#include <boost/lexical_cast.hpp>

Diagnostic::DiagPtr Diagnostic::sm_diag;

void Diagnostic::Initialise()
{
    sm_diag.reset(new Diagnostic());
}

Diagnostic::Diagnostic() :
    m_textTimer(0.0f),
    m_text(nullptr),
    m_light(nullptr),
    m_render(true),
    m_previousFPS(-1)
{
    m_bgroundCol.set(200,255,255,255); //partially transparent
    m_clearCol.set(0,0,0,0);

    // Light diagnostics text box
    const int border = 10;
    const int boxWidth = 125;
    const int boxHeight = 200;
    m_light = Gui()->addStaticText(L"", rect<s32>(border,border, 
        WINDOW_WIDTH-border,WINDOW_HEIGHT-border)); 
    m_light->setBackgroundColor(m_clearCol);
    m_light->setMaxSize(dimension2du(boxWidth, boxHeight));
    m_light->setDrawBackground(false);

    // General diagnostics text box
    const int width = 100;
    const int height = 30;
    m_text = Gui()->addStaticText(L"", rect<s32>(
        WINDOW_WIDTH/2-width,border, 
        WINDOW_WIDTH/2+width,border+height)); 
    m_text->setBackgroundColor(m_clearCol);
    m_text->setDrawBackground(false);
    m_text->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);
}

Diagnostic::~Diagnostic()
{
}

void Diagnostic::ShowDiagnosticText(const std::wstring& text)
{
    m_text->setBackgroundColor(m_bgroundCol);
    m_text->setDrawBackground(true);
    m_text->setText(text.c_str());
    m_runTimer = true;
}

void Diagnostic::Render(float deltatime, const stringw& lighttext)
{
    if(m_render)
    {
        int fps = Driver()->getFPS();
        if(m_previousFPS != fps)
        {
            Device()->setWindowCaption((L"FPS: " + 
                boost::lexical_cast<std::wstring>(fps)).c_str());
            m_previousFPS = fps;
        }

        m_light->setDrawBackground(!lighttext.empty());
        m_light->setBackgroundColor(!lighttext.empty() ? m_bgroundCol : m_clearCol);
        m_light->setText(lighttext.c_str());

        if(m_runTimer)
        {
            m_textTimer += deltatime;
            if(m_textTimer >= 1.0)
            {
                m_text->setDrawBackground(false);
                m_text->setBackgroundColor(m_clearCol);
                m_text->setText(L"");
                m_textTimer = 0.0f;
                m_runTimer = false;
            }
        }
        Gui()->drawAll();
    }
}