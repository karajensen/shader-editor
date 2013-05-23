#include "diagnostic.h"
#include "generated_shader.h"
#include "post_shader.h"
#include <boost/lexical_cast.hpp>
#include <algorithm>

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

    const int lineHeight = 30;
    const int border = 10;

    // Light diagnostics text box
    const int lightWidth = 125;
    const int lightHeight = 200;
    m_light = Gui()->addStaticText(L"", rect<s32>(border,border, 
        border+lightWidth, border+lightHeight)); 
    m_light->setBackgroundColor(m_clearCol);
    m_light->setDrawBackground(false);

    // General diagnostics text box
    const int generalWidth = 100;
    m_text = Gui()->addStaticText(L"", rect<s32>(WINDOW_WIDTH/2-generalWidth, border, 
        WINDOW_WIDTH/2+generalWidth, border+lineHeight)); 
    m_text->setBackgroundColor(m_clearCol);
    m_text->setDrawBackground(false);
    m_text->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    // Check box gui background
    const int listLineHeight = 20;
    const int startY = border;
    const int startX = WINDOW_WIDTH-100;
    const int endX = WINDOW_WIDTH-border;
    const int endY = startY+(GeneratedShader::MAX_EDITABLE*lineHeight)
        +(PostShader::MAX_TEXTURES*listLineHeight);
    
    rect<s32> bgroundRect(startX-border, startY-border, endX+border, endY+border);
    m_componentBground = Gui()->addStaticText(L"",bgroundRect);
    m_componentBground->setBackgroundColor(m_bgroundCol);

    // Shader components text box
    int currentY = startY;
    for(unsigned int i = 0; i < GeneratedShader::MAX_EDITABLE; ++i)
    {
        rect<s32> checkboxRect(startX, currentY, endX, currentY+lineHeight);
        currentY += lineHeight;

        auto checkFn = std::bind(&GeneratedShader::SetComponentVisibility, i, std::placeholders::_1);

        m_checkBoxes.push_back(CheckBoxPtr(new CheckBox(checkFn, checkboxRect, 
            GeneratedShader::GetComponentDescription(i))));
    }

    // Post shader output texture options
    currentY += lineHeight;
    rect<s32> listBoxRect(startX, currentY, endX, currentY+(PostShader::MAX_TEXTURES*listLineHeight));
    m_outTextureList = Gui()->addListBox(listBoxRect);
    m_outTextureList->setText(L"Output Texture");

    for(unsigned int i = 0; i < PostShader::MAX_TEXTURES; ++i)
    {
        m_outTextureList->addItem(PostShader::GetTextureDescription(i).c_str());
    }
    m_outTextureList->setSelected(0);
}

void Diagnostic::ToggleShowDiagnostics()
{
    m_render = !m_render;
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
        // Render FPS
        int fps = Driver()->getFPS();
        if(m_previousFPS != fps)
        {
            Device()->setWindowCaption((L"FPS: " + 
                boost::lexical_cast<std::wstring>(fps)).c_str());
            m_previousFPS = fps;
        }

        // Render light editor diagnostics
        m_light->setDrawBackground(!lighttext.empty());
        m_light->setBackgroundColor(!lighttext.empty() ? m_bgroundCol : m_clearCol);
        m_light->setText(lighttext.c_str());

        // Update text boxes
        std::for_each(m_checkBoxes.begin(), m_checkBoxes.end(), 
            [](const CheckBoxPtr& checkbox){ checkbox->Update(); });

        // Update list box
        PostShader::SetTextureVisibility(m_outTextureList->getSelected(), true);

        // Update general text diagnostics
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

CheckBox::CheckBox(CheckFn checkFn, rect<s32> textBox, const stringw& text) :
    m_onCheckFn(checkFn)
{
    m_checkbox = Gui()->addCheckBox(true, textBox);
    m_checkbox->setText(text.c_str());
    m_onCheckFn(true);
}

void CheckBox::Update()
{
    m_onCheckFn(m_checkbox->isChecked());
}