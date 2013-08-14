////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "diagnostic.h"
#include "postshader.h"
#include "shadereditor.h"
#include "lighteditor.h"
#include "camera.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

namespace
{
    const int BORDER = 10;                  ///< Border to edge of screen
    SColor BGROUND_COLOR(100,255,255,255);  ///< Colour for box background
    SColor CLEAR_COLOR(0,0,0,0);            ///< Colour for a clean background
}

Diagnostic::Diagnostic(EnginePtr engine, 
    boost::shared_ptr<ShaderEditor> shader, 
    boost::shared_ptr<PostShader> postshader,
    boost::shared_ptr<LightEditor> light) :
        m_textTimer(0.0f),
        m_render(true),
        m_previousFPS(-1),
        m_engine(engine),
        m_tweakbar(nullptr),
        m_text(nullptr),
        m_postshader(postshader),
        m_shader(shader),
        m_light(light)
{
    const int textwidth = 100;
    const int textheight = 30;

    m_text = m_engine->gui->addStaticText(L"", rect<s32>(WINDOW_WIDTH/2-textwidth, 
        BORDER, WINDOW_WIDTH/2+textwidth, BORDER+textheight)); 
    
    m_text->setBackgroundColor(CLEAR_COLOR);
    m_text->setDrawBackground(false);
    m_text->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    const int tweakwidth = 300;
    const int tweakheight = 400;

    TwInit(TW_OPENGL, NULL);
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_tweakbar = TwNewBar("Shader Scenery");

    for(int i = 0; i < ShaderEditor::MAX_EDITABLE; ++i)
    {
        ShaderEditor::EditableComponent component = 
            static_cast<ShaderEditor::EditableComponent>(i);

        TwAddVarRW(m_tweakbar, m_shader->GetComponentDescription(component).c_str(),
            TW_TYPE_FLOAT, m_shader->GetComponentAddress(component), "");
    }

    for(int i = 0; i < PostShader::MAX_EDITABLE; ++i)
    {
        PostShader::EditableComponent component = 
            static_cast<PostShader::EditableComponent>(i);

        TwAddVarRW(m_tweakbar, m_postshader->GetComponentDescription(component).c_str(),
            TW_TYPE_FLOAT, m_postshader->GetComponentAddress(component), "");
    }

    for(int i = 0; i < PostShader::MAX_TEXTURES; ++i)
    {
        PostShader::OutputTexture texture = 
            static_cast<PostShader::OutputTexture>(i);

    }

}

Diagnostic::~Diagnostic()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Diagnostic::ToggleShowDiagnostics()
{
    m_render = !m_render;
}

void Diagnostic::ShowDiagnosticText(const std::wstring& text)
{
    m_text->setBackgroundColor(BGROUND_COLOR);
    m_text->setDrawBackground(true);
    m_text->setText(text.c_str());
    m_runTimer = true;
}

void Diagnostic::Render(float deltatime)
{
    if(m_render)
    {
        TwDraw();

        int fps = m_engine->driver->getFPS();
        if(m_previousFPS != fps)
        {
            m_engine->device->setWindowCaption((L"FPS: " + 
                boost::lexical_cast<std::wstring>(fps)).c_str());
            m_previousFPS = fps;
        }

        if(m_runTimer)
        {
            m_textTimer += deltatime;
            if(m_textTimer >= 1.0)
            {
                m_text->setDrawBackground(false);
                m_text->setBackgroundColor(CLEAR_COLOR);
                m_text->setText(L"");
                m_textTimer = 0.0f;
                m_runTimer = false;
            }
        }

        m_engine->gui->drawAll();
    }
}
