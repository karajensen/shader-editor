////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "diagnostic.h"
#include "postshader.h"
#include "shadereditor.h"
#include "lighteditor.h"
#include "camera.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace
{
    /**
    * Anttweakbar button callback for setting the output texture
    * @param clientData The user given data
    */
    void TW_CALL SetVisibleTexture(void *clientData)
    {
        (*static_cast<Diagnostic::TextureFunctor*>(clientData))();
    }

    /**
    * Anttweakbar button callback for selecting the next light
    * @param clientData The user given data
    */
    void TW_CALL SelectNextLight(void *clientData)
    {
        (static_cast<LightEditor*>(clientData))->SelectNextLight();
    }

    const std::string BARNAME = "TweakBar";  ///< Name of the tweak bar
    const int BORDER = 10;                   ///< Border to edge of screen
    SColor BGROUND_COLOR(100,255,255,255);   ///< Colour for box background
    SColor CLEAR_COLOR(0,0,0,0);             ///< Colour for a clean background
}

Diagnostic::Diagnostic(EnginePtr engine, 
    boost::shared_ptr<ShaderEditor> shader, 
    boost::shared_ptr<PostShader> postshader,
    boost::shared_ptr<LightEditor> light,
    boost::shared_ptr<Camera> camera) :
        m_textTimer(0.0f),
        m_render(true),
        m_previousFPS(-1),
        m_engine(engine),
        m_tweakbar(nullptr),
        m_text(nullptr),
        m_postshader(postshader),
        m_shader(shader),
        m_light(light),
        m_camera(camera)
{
    const int textwidth = 100;
    const int textheight = 30;

    m_text = m_engine->gui->addStaticText(L"", rect<s32>(WINDOW_WIDTH/2-textwidth, 
        BORDER, WINDOW_WIDTH/2+textwidth, BORDER+textheight)); 
    
    m_text->setBackgroundColor(CLEAR_COLOR);
    m_text->setDrawBackground(false);
    m_text->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    TwInit(TW_OPENGL, NULL);
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_tweakbar = TwNewBar(BARNAME.c_str());
    
    std::ostringstream stream;
    stream << " " << BARNAME << " label='Shader Scenery' " 
        << "position='" << BORDER << " " << BORDER << "' "
        << "size='200 " << WINDOW_HEIGHT-BORDER*2 << "' "
        << "alpha=180 text=light valueswidth=70 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=false "
        << "fontsize=2 fontresizable=false ";

    TwDefine(stream.str().c_str());

    // Output texture specific diagnostics
    for(int i = 0; i < PostShader::MAX_TEXTURES; ++i)
    {
        PostShader::OutputTexture texture = 
            static_cast<PostShader::OutputTexture>(i);

        std::string label(" group='Output Textures' label='" 
            + m_postshader->GetTextureDescription(texture) + "'");

        m_buttonCallbacks[i].postshader = m_postshader;
        m_buttonCallbacks[i].texture = texture;

        TwAddButton(m_tweakbar, (boost::lexical_cast<std::string>(i)+"Texture").c_str(), 
            &SetVisibleTexture, &m_buttonCallbacks[i], label.c_str());
    }

    // Shader specific diagnostics
    std::string shaderDesc(" group='Scene Shader' step=0.01 precision=2");
    for(int i = 0; i < ShaderEditor::MAX_EDITABLE; ++i)
    {
        ShaderEditor::EditableComponent component = 
            static_cast<ShaderEditor::EditableComponent>(i);

        TwAddVarRW(m_tweakbar, m_shader->GetComponentDescription(component),
            TW_TYPE_FLOAT, m_shader->GetComponentAddress(component), shaderDesc.c_str());
    }

    // Post shader specific diagnostics
    std::string postDesc(" group='Post Shader' step=0.01 precision=2");
    for(int i = 0; i < PostShader::MAX_EDITABLE; ++i)
    {
        PostShader::EditableComponent component = 
            static_cast<PostShader::EditableComponent>(i);

        TwAddVarRW(m_tweakbar, m_postshader->GetComponentDescription(component),
            TW_TYPE_FLOAT, m_postshader->GetComponentAddress(component), postDesc.c_str());
    }

    // Camera specific diagnostics
    std::string cameraDesc(" group='Camera' step=1.0 precision=2");
    
    TwAddVarRW(m_tweakbar, m_camera->GetComponentDescription(Camera::NEAR_VALUE), 
        TW_TYPE_FLOAT, m_camera->GetComponentAddress(Camera::NEAR_VALUE), cameraDesc.c_str());

    TwAddVarRW(m_tweakbar, m_camera->GetComponentDescription(Camera::FAR_VALUE), 
        TW_TYPE_FLOAT, m_camera->GetComponentAddress(Camera::FAR_VALUE), cameraDesc.c_str());

    // Lighting specific diagnostics
    std::string lightDesc(" group='Light'");
    LightEditor::LightData& lightdata = m_light->GetSelectedLightData();

    TwStructMember positionStruct[] = 
    { 
        { "x", TW_TYPE_FLOAT, 0,             " group='Position' Step=1.0 " },
        { "y", TW_TYPE_FLOAT, sizeof(float), " group='Position' Step=1.0 " },
        { "z", TW_TYPE_FLOAT, sizeof(float), " group='Position' Step=1.0 " }
    };
    TwType positionType = TwDefineStruct("LightPosition", 
        positionStruct, 3, sizeof(float)*3, nullptr, nullptr);

    TwStructMember attenuationStruct[] = 
    { 
        { "att0", TW_TYPE_FLOAT, 0,             " group='Attenuation' Step=0.01 Precision=2 " },
        { "att1", TW_TYPE_FLOAT, sizeof(float), " group='Attenuation' Step=0.01 Precision=2 " },
        { "att2", TW_TYPE_FLOAT, sizeof(float), " group='Attenuation' Step=0.01 Precision=2 " }
    };
    TwType attenuationType = TwDefineStruct("LightAttenuation", 
        attenuationStruct, 3, sizeof(float)*3, nullptr, nullptr);

    TwAddVarRW(m_tweakbar, "Position", positionType, 
        &lightdata.position, lightDesc.c_str());

    TwAddVarRW(m_tweakbar, "Attenuation", attenuationType,
        &lightdata.attenuation, lightDesc.c_str());

    TwAddVarRW(m_tweakbar, "DiffuseColor", TW_TYPE_COLOR3F, 
        &lightdata.diffuse, (lightDesc + " label='Diffuse'").c_str());

    TwAddVarRW(m_tweakbar, "SpecularColor", TW_TYPE_COLOR3F, 
        &lightdata.specular, (lightDesc + " label='Specular'").c_str());

    TwAddVarRW(m_tweakbar, "CastsShadows", TW_TYPE_BOOLCPP,
        &lightdata.castsShadows, (lightDesc + " label='Shadows'").c_str());

    TwAddButton(m_tweakbar, "Next Light",
        &SelectNextLight, m_light.get(), lightDesc.c_str());
}

Diagnostic::~Diagnostic()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Diagnostic::TextureFunctor::operator()()
{
    postshader->SetVisibleTexture(texture);
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

void Diagnostic::HandleInputEvent(const SEvent& inputEvent)
{
    int handled = 0;

    switch(inputEvent.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        if(inputEvent.KeyInput.PressedDown)
        {
            std::wstring wideChar;
            wideChar[0] = inputEvent.KeyInput.Char;
            std::string character(wideChar.begin(), wideChar.end());
            handled = TwKeyPressed(character[0], 0);
        }
    case irr::EET_MOUSE_INPUT_EVENT:
        switch(inputEvent.MouseInput.Event)
        {
        case EMIE_LMOUSE_LEFT_UP:
            handled = TwMouseButton(TW_MOUSE_RELEASED,TW_MOUSE_LEFT);
            break;
        case EMIE_LMOUSE_PRESSED_DOWN:
            handled = TwMouseButton(TW_MOUSE_PRESSED,TW_MOUSE_LEFT);
            break;
        case EMIE_RMOUSE_LEFT_UP:
            handled = TwMouseButton(TW_MOUSE_RELEASED,TW_MOUSE_RIGHT);
            break;
        case EMIE_RMOUSE_PRESSED_DOWN:
            handled = TwMouseButton(TW_MOUSE_PRESSED,TW_MOUSE_RIGHT);
            break;;
        case EMIE_MOUSE_MOVED:
            handled = TwMouseMotion(inputEvent.MouseInput.X, inputEvent.MouseInput.Y);
            break;
        }
    }
    m_engine->scene->getActiveCamera()->setInputReceiverEnabled(handled == 0);
}

void Diagnostic::Render(float deltatime)
{
    if(m_render)
    {
        TwDraw();

        TwSetParam(m_tweakbar, "Camera", "label", 
            TW_PARAM_CSTRING, 1, m_camera->GetCameraType());

        TwSetParam(m_tweakbar, "Light", "label", 
            TW_PARAM_CSTRING, 1, m_light->GetLightType());

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
