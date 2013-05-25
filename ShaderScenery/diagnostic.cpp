#include "diagnostic.h"
#include "generated_shader.h"
#include "post_shader.h"
#include <boost/lexical_cast.hpp>
#include <algorithm>

namespace
{
    const int BORDER = 10;                  ///< Border for diagnosic boxes
    SColor BGROUND_COLOR(200,255,255,255);  ///< Colour for diag box background
    SColor LABEL_COLOR(200,200,200,200);    ///< Colour for diag box labels
    SColor CLEAR_COLOR(0,0,0,0);            ///< Colour for a clean background
}

Diagnostic::DiagPtr Diagnostic::sm_diag;

Diagnostic::Diagnostic() :
    m_textTimer(0.0f),
    m_render(true),
    m_previousFPS(-1),
    m_text(nullptr),
    m_light(nullptr),
    m_outTextureList(nullptr)
{
    // General diagnostics text box
    const int generalWidth = 100;
    const int generalHeight = 30;

    m_text = Gui()->addStaticText(L"", rect<s32>(WINDOW_WIDTH/2-generalWidth, 
        BORDER, WINDOW_WIDTH/2+generalWidth, BORDER+generalHeight)); 

    m_text->setBackgroundColor(CLEAR_COLOR);
    m_text->setDrawBackground(false);
    m_text->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    InitialiseShaderDiagnostics();
    InitialiseLightDiagnostics();
}

void Diagnostic::InitialiseShaderDiagnostics()
{
    const int sectionSpacing = 30;
    const int spinBoxSpacing = 4;
    const int spinBoxLineHeight = 20;
    const int spinTextLineHeight = 12;
    const int startX = WINDOW_WIDTH-110;
    const int endX = WINDOW_WIDTH-20;
    int startY = 20;
    int currentY = startY;

    auto createBackground = [&](const int endY)
    {
        rect<s32> bgroundRect(startX-BORDER, currentY-BORDER, endX+BORDER, endY+BORDER);
        auto* bground = Gui()->addStaticText(L"",bgroundRect);
        bground->setBackgroundColor(BGROUND_COLOR);
        m_bgrounds.push_back(bground);
    };

    auto createSpinBox = [&](unsigned int items, 
        std::function<void(unsigned int, float)> visFn,
        std::function<stringw(unsigned int)> descFn)
    {
        for(unsigned int i = 0; i < items; ++i)
        {
            rect<s32> textRect(startX, currentY, endX, currentY+spinTextLineHeight);
            currentY += spinTextLineHeight;
            rect<s32> spinRect(startX, currentY, endX, currentY+spinBoxLineHeight);
            currentY += spinBoxLineHeight;

            auto valueFn = std::bind(visFn, i, std::placeholders::_1);
            m_spinBoxes.push_back(Diagnostic::SpinBoxPtr(new SpinBox(
                valueFn, spinRect, textRect, descFn(i))));

            currentY += spinBoxSpacing;
        }
    };

    // Shader components spin box
    createBackground(currentY - spinBoxSpacing + (GeneratedShader::MAX_EDITABLE *
        (spinBoxLineHeight + spinTextLineHeight + spinBoxSpacing)));

    createSpinBox(GeneratedShader::MAX_EDITABLE, 
        GeneratedShader::SetComponentVisibility, 
        GeneratedShader::GetComponentDescription);

    // Post shader components spin box background
    currentY += sectionSpacing;
    createBackground(currentY - spinBoxSpacing + (PostShader::MAX_EDITABLE *
        (spinBoxLineHeight + spinTextLineHeight + spinBoxSpacing)));

    createSpinBox(PostShader::MAX_EDITABLE, 
        PostShader::SetComponentVisibility, 
        PostShader::GetComponentDescription);

    // Post shader output texture options
    currentY += sectionSpacing;
    const int listLineHeight = 14;
    const int listEndY = currentY+(PostShader::MAX_TEXTURES*listLineHeight);
    createBackground(listEndY);

    rect<s32> listBoxRect(startX, currentY, endX, listEndY);
    m_outTextureList = Gui()->addListBox(listBoxRect);
    for(unsigned int i = 0; i < PostShader::MAX_TEXTURES; ++i)
    {
        m_outTextureList->addItem(PostShader::GetTextureDescription(i).c_str());
    }
    m_outTextureList->setSelected(0);
}

void Diagnostic::InitialiseLightDiagnostics()
{
    const int lightWidth = 125;
    const int lightHeight = 200;
    m_light = Gui()->addStaticText(L"", rect<s32>(BORDER, BORDER, 
        BORDER+lightWidth, BORDER+lightHeight)); 
    m_light->setBackgroundColor(CLEAR_COLOR);
    m_light->setDrawBackground(false);
}

void Diagnostic::Initialise()
{
    sm_diag.reset(new Diagnostic());
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
        m_light->setBackgroundColor(!lighttext.empty() ? BGROUND_COLOR : CLEAR_COLOR);
        m_light->setText(lighttext.c_str());

        // Update text boxes
        std::for_each(m_spinBoxes.begin(), m_spinBoxes.end(), 
            [](const SpinBoxPtr& spinbox){ spinbox->Update(); });

        // Update list box
        PostShader::SetTextureVisibility(m_outTextureList->getSelected(), true);

        // Update general text diagnostics
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

        Gui()->drawAll();
    }
}

SpinBox::SpinBox(ValueFn valueFn, rect<s32> spinRect, rect<s32> textRect, const stringw& text) :
    m_updateValueFn(valueFn)
{
    const float initialValue = 1.0f;
    const float stepSize = 0.05f;
    const float maxValue = 10.0f;
    const int precision = 2;

    m_text = Gui()->addStaticText(text.c_str(), textRect);
    m_text->setBackgroundColor(LABEL_COLOR);

    m_spinbox = Gui()->addSpinBox(text.c_str(), spinRect);
    m_spinbox->setValue(initialValue);
    m_spinbox->setRange(0.0f, maxValue);
    m_spinbox->setDecimalPlaces(precision);
    m_spinbox->setStepSize(stepSize);
    m_updateValueFn(initialValue);
}

void SpinBox::Update()
{
    m_updateValueFn(m_spinbox->getValue());
}