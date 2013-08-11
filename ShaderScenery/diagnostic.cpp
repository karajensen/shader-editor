#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "diagnostic.h"
#include "generatedshader.h"
#include "postshader.h"
#include "lighteditor.h"
#include "camera.h"

namespace
{
    const int SPIN_SPACING = 4;             ///< Spacing between successive spin boxes
    const int SPINBOX_LINE_HEIGHT = 20;     ///< Height of the spin box
    const int SPINTEXT_LINE_HEIGHT = 12;    ///< Height of the spin box text
    const int BORDER = 10;                  ///< Border for boxes
    SColor BGROUND_COLOR(100,255,255,255);  ///< Colour for box background
    SColor LABEL_COLOR(200,200,200,200);    ///< Colour for box labels
    SColor CLEAR_COLOR(0,0,0,0);            ///< Colour for a clean background
}

Diagnostic::Diagnostic(EnginePtr engine) :
    m_textTimer(0.0f),
    m_render(true),
    m_previousFPS(-1),
    m_engine(engine),
    m_generalText(nullptr)
{
    // General diagnostics text box
    const int generalWidth = 100;
    const int generalHeight = 30;

    m_generalText = m_engine->gui->addStaticText(L"", rect<s32>(WINDOW_WIDTH/2-generalWidth, 
        BORDER, WINDOW_WIDTH/2+generalWidth, BORDER+generalHeight)); 
    
    m_generalText->setBackgroundColor(CLEAR_COLOR);
    m_generalText->setDrawBackground(false);
    m_generalText->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    InitialiseDiagnosticGUI();
    SetSpinBoxRanges();
    UpdateLightDiagnostics();
    UpdateCameraDiagnostics();
}

void Diagnostic::InitialiseDiagnosticGUI()
{
    //const int sectionSpacing = 30;
    //int startX = WINDOW_WIDTH-110;
    //int endX = WINDOW_WIDTH-20;
    //int currentY = 20;
    //
    //auto createBackground = [&](const int endY)
    //{
    //    rect<s32> bgroundRect(startX-BORDER, currentY-BORDER, endX+BORDER, endY+BORDER);
    //    auto* bground = Gui()->addStaticText(L"",bgroundRect);
    //    bground->setBackgroundColor(BGROUND_COLOR);
    //    m_bgrounds.push_back(bground);
    //};
    //
    //auto createSpinBoxLabel = [&](IGUIStaticText** text, const int labelHeight)
    //{
    //    rect<s32> rect(startX, currentY, endX, currentY+labelHeight);
    //    *text = Gui()->addStaticText(L"", rect);
    //    (*text)->setBackgroundColor(LABEL_COLOR);
    //    currentY += labelHeight+SPIN_SPACING;
    //};
    //
    //auto createSpinBox = [&](SpinBoxVector& container, unsigned int items, 
    //    std::function<void(unsigned int, float)> visFn,
    //    std::function<stringw(unsigned int)> descFn)
    //{
    //    for(unsigned int i = 0; i < items; ++i)
    //    {
    //        rect<s32> textRect(startX, currentY, endX, currentY+SPINTEXT_LINE_HEIGHT);
    //        currentY += SPINTEXT_LINE_HEIGHT;
    //        rect<s32> spinRect(startX, currentY, endX, currentY+SPINBOX_LINE_HEIGHT);
    //        currentY += SPINBOX_LINE_HEIGHT;
    //
    //        auto valueFn = std::bind(visFn, i, std::placeholders::_1);
    //        container.push_back(Diagnostic::SpinBoxPtr(new SpinBox(
    //            valueFn, spinRect, textRect, descFn(i))));
    //
    //        currentY += SPIN_SPACING;
    //    }
    //};
    //
    //// Shader components spin box
    //createBackground(currentY - SPIN_SPACING + (GeneratedShader::MAX_EDITABLE *
    //    (SPINBOX_LINE_HEIGHT + SPINTEXT_LINE_HEIGHT + SPIN_SPACING)));
    //
    //createSpinBox(m_shaderSpinBoxes, GeneratedShader::MAX_EDITABLE, 
    //    GeneratedShader::SetComponentVisibility, GeneratedShader::GetComponentDescription);
    //
    //// Post shader components spin box background
    //currentY += sectionSpacing;
    //createBackground(currentY - SPIN_SPACING + (PostShader::MAX_EDITABLE *
    //    (SPINBOX_LINE_HEIGHT + SPINTEXT_LINE_HEIGHT + SPIN_SPACING)));
    //
    //createSpinBox(m_shaderSpinBoxes, PostShader::MAX_EDITABLE, 
    //    PostShader::SetComponentVisibility, PostShader::GetComponentDescription);
    //
    //// Post shader output texture options
    //currentY += sectionSpacing;
    //const int listLineHeight = 14;
    //const int listHeight = PostShader::MAX_TEXTURES*listLineHeight;
    //createBackground(currentY+listHeight);
    //
    //rect<s32> listBoxRect(startX, currentY, endX, currentY+listHeight);
    //auto* listBground = Gui()->addStaticText(L"", listBoxRect);
    //listBground->setBackgroundColor(LABEL_COLOR);
    //m_bgrounds.push_back(listBground);
    //m_outTextureList = Gui()->addListBox(listBoxRect);
    //
    //for(unsigned int i = 0; i < PostShader::MAX_TEXTURES; ++i)
    //{
    //    m_outTextureList->addItem(PostShader::GetTextureDescription(i).c_str());
    //}
    //m_outTextureList->setSelected(0);
    //currentY += listHeight;
    //
    //// Normal shader/camera options
    //currentY += sectionSpacing;
    //const int cameraLabelHeight = 12;
    //const int cameraSpinBoxes = 2;
    //createBackground(currentY + cameraLabelHeight + (cameraSpinBoxes*
    //    (SPINBOX_LINE_HEIGHT + SPIN_SPACING + SPINTEXT_LINE_HEIGHT)));
    //
    //createSpinBoxLabel(&m_cameraLabel, cameraLabelHeight);
    //
    //createSpinBox(m_cameraSpinBoxes, cameraSpinBoxes, 
    //    Camera::SetComponentValue, Camera::GetComponentDescription);
    //
    //// Light Diagnostics
    //const auto& lightPtr = LightEditor::Get();
    //const int lightLabelHeight = 23;
    //currentY = 20;
    //startX = 20;
    //endX = 110;
    //
    //createBackground(currentY + lightLabelHeight + (LightEditor::MAX_ATTRIBUTES *
    //    (SPINBOX_LINE_HEIGHT + SPINTEXT_LINE_HEIGHT + SPIN_SPACING)));
    //
    //createSpinBoxLabel(&m_lightLabel, lightLabelHeight);
    //
    //auto valueFn = std::bind(&LightEditor::SetAttributeValue, 
    //    lightPtr.get(), std::placeholders::_1, std::placeholders::_2);
    //
    //auto descFn = std::bind(&LightEditor::GetAttributeDescription, 
    //    lightPtr.get(), std::placeholders::_1);
    //
    //createSpinBox(m_lightSpinBoxes, LightEditor::MAX_ATTRIBUTES, valueFn, descFn);
}

void Diagnostic::SetSpinBoxRanges()
{
   //const float cameraStep = 0.1f;
   //const float shadowStep = 1.0f;
   //const float attStep = 0.0001f;
   //const float posStep = 0.5f;
   //const float colorStep = 0.001f;
   //const float maxColor = 10.0f;
   //const int lightPrec = 4;
   //const int cameraPrec = 2;
   //
   //m_lightSpinBoxes[LightEditor::ATTENUATION_X]->SetRange(0.0f, 1.0f, attStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::ATTENUATION_Y]->SetRange(0.0f, 1.0f, attStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::ATTENUATION_Z]->SetRange(0.0f, 1.0f, attStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::POSITION_X]->SetRange(-FLT_MAX, FLT_MAX, posStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::POSITION_Y]->SetRange(-FLT_MAX, FLT_MAX, posStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::POSITION_Z]->SetRange(-FLT_MAX, FLT_MAX, posStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::COLOR_R]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::COLOR_G]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::COLOR_B]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::SPECCOLOR_R]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::SPECCOLOR_G]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::SPECCOLOR_B]->SetRange(0.0f, maxColor, colorStep, lightPrec);
   //m_lightSpinBoxes[LightEditor::SHADOWS]->SetRange(0.0f, 1.0f, shadowStep, lightPrec);
   //m_cameraSpinBoxes[Camera::FAR_VALUE]->SetRange(-FLT_MAX, FLT_MAX, cameraStep, cameraPrec);
   //m_cameraSpinBoxes[Camera::NEAR_VALUE]->SetRange(-FLT_MAX, FLT_MAX, cameraStep, cameraPrec);
}

void Diagnostic::UpdateLightDiagnostics()
{
    //const auto& lightdata = LightEditor::Get()->GetSelectedLightData();
    //m_lightSpinBoxes[LightEditor::ATTENUATION_X]->SetValue(lightdata.Attenuation.X);
    //m_lightSpinBoxes[LightEditor::ATTENUATION_Y]->SetValue(lightdata.Attenuation.Y);
    //m_lightSpinBoxes[LightEditor::ATTENUATION_Z]->SetValue(lightdata.Attenuation.Z);
    //m_lightSpinBoxes[LightEditor::POSITION_X]->SetValue(lightdata.Position.X);
    //m_lightSpinBoxes[LightEditor::POSITION_Y]->SetValue(lightdata.Position.Y);
    //m_lightSpinBoxes[LightEditor::POSITION_Z]->SetValue(lightdata.Position.Z);
    //m_lightSpinBoxes[LightEditor::COLOR_R]->SetValue(lightdata.DiffuseColor.r);
    //m_lightSpinBoxes[LightEditor::COLOR_G]->SetValue(lightdata.DiffuseColor.g);
    //m_lightSpinBoxes[LightEditor::COLOR_B]->SetValue(lightdata.DiffuseColor.b);
    //m_lightSpinBoxes[LightEditor::SPECCOLOR_R]->SetValue(lightdata.SpecularColor.r);
    //m_lightSpinBoxes[LightEditor::SPECCOLOR_G]->SetValue(lightdata.SpecularColor.g);
    //m_lightSpinBoxes[LightEditor::SPECCOLOR_B]->SetValue(lightdata.SpecularColor.b);
    //m_lightSpinBoxes[LightEditor::SHADOWS]->SetValue(lightdata.CastShadows ? 1.0f : 0.0f);   
}

void Diagnostic::UpdateCameraDiagnostics()
{
    //m_cameraSpinBoxes[Camera::NEAR_VALUE]->SetValue(Scene()->getActiveCamera()->getNearValue());
    //m_cameraSpinBoxes[Camera::FAR_VALUE]->SetValue(Scene()->getActiveCamera()->getFarValue());
}

void Diagnostic::ToggleShowDiagnostics()
{
    m_render = !m_render;
}

void Diagnostic::ShowDiagnosticText(const std::wstring& text)
{
    m_generalText->setBackgroundColor(BGROUND_COLOR);
    m_generalText->setDrawBackground(true);
    m_generalText->setText(text.c_str());
    m_runTimer = true;
}

void Diagnostic::Render(float deltatime)
{
    if(m_render)
    {
        int fps = m_engine->driver->getFPS();
        if(m_previousFPS != fps)
        {
            m_engine->device->setWindowCaption((L"FPS: " + 
                boost::lexical_cast<std::wstring>(fps)).c_str());
            m_previousFPS = fps;
        }

        //auto updateSpinBox = [](const SpinBoxPtr& spinbox){ spinbox->Update(); };
        //std::for_each(m_shaderSpinBoxes.begin(), m_shaderSpinBoxes.end(), updateSpinBox);
        //std::for_each(m_lightSpinBoxes.begin(), m_lightSpinBoxes.end(), updateSpinBox);
        //std::for_each(m_cameraSpinBoxes.begin(), m_cameraSpinBoxes.end(), updateSpinBox);
        //
        //m_lightLabel->setText(LightEditor::Get()->GetSelectedLightDescription().c_str());
        //
        //m_cameraLabel->setText(Camera::GetCameraType().c_str());
        //
        //PostShader::SetVisibleTexture(m_outTextureList->getSelected());

        if(m_runTimer)
        {
            m_textTimer += deltatime;
            if(m_textTimer >= 1.0)
            {
                m_generalText->setDrawBackground(false);
                m_generalText->setBackgroundColor(CLEAR_COLOR);
                m_generalText->setText(L"");
                m_textTimer = 0.0f;
                m_runTimer = false;
            }
        }

        m_engine->gui->drawAll();
    }
}

//SpinBox::SpinBox(ValueFn valueFn, rect<s32> spinRect, rect<s32> textRect, const stringw& text) :
//    m_updateValueFn(valueFn)
//{
//    m_text = Gui()->addStaticText(text.c_str(), textRect);
//    m_text->setBackgroundColor(LABEL_COLOR);
//    m_spinbox = Gui()->addSpinBox(text.c_str(), spinRect);
//    
//    SetRange(0.0f, 10.0f, 0.05f, 2);
//    SetValue(1.0f);
//}

//void SpinBox::SetRange(float min, float max, float stepSize, int precision)
//{
//    m_spinbox->setRange(min, max);
//    m_spinbox->setStepSize(stepSize);
//    m_spinbox->setDecimalPlaces(precision);
//}

//void SpinBox::SetValue(float value)
//{
//    m_spinbox->setValue(value);
//}

//void SpinBox::Update()
//{
//    m_updateValueFn(m_spinbox->getValue());
//}