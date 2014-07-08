////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "cache.h"
#include "qt/project/editor.h"
#include "qt/project/tweaker.h"
#include "qt/include/QtWidgets/qapplication.h"

Gui::Gui(std::shared_ptr<Cache> cache) :
    m_cache(cache)
{
}

Gui::~Gui()
{
}

void Gui::Run(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");
    QApplication app(argc, argv);

    Editor editor;
    editor.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    editor.show();

    Tweaker tweaker;
    tweaker.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    tweaker.show();

    SetSignalCallbacks(tweaker, editor);
    
    while(m_cache->ApplicationRunning.Get())
    {
        app.processEvents();
        UpdateCache(tweaker, editor);
    }

    app.exit();
    Logger::LogInfo("Exiting Qt");
}

GuiPage Gui::ConvertStringToPage(const std::string& page)
{
    if(page == "Scene")
    {
        return SCENE;
    }
    else if(page == "Mesh")
    {
        return MESH;
    }
    else if(page == "Post")
    {
        return POST;
    }
    else if(page == "Light")
    {
        return LIGHT;
    }
    return NO_PAGE;
}

void Gui::SetSignalCallbacks(Tweaker& tweaker, Editor& editor)
{
    SignalCallbacks callbacks;

    callbacks.SetLightPositionX =   [&](float x){ m_cache->LightPosition.SetX(x); };
    callbacks.SetLightPositionY =   [&](float y){ m_cache->LightPosition.SetY(y); };
    callbacks.SetLightPositionZ =   [&](float z){ m_cache->LightPosition.SetZ(z); };
    callbacks.SetLightAttX =        [&](float x){ m_cache->LightAttenuation.SetX(x); };
    callbacks.SetLightAttY =        [&](float y){ m_cache->LightAttenuation.SetY(y); };
    callbacks.SetLightAttZ =        [&](float z){ m_cache->LightAttenuation.SetZ(z); };
    callbacks.SetLightDiffuseR =    [&](float r){ m_cache->LightDiffuse.SetR(r); };
    callbacks.SetLightDiffuseG =    [&](float g){ m_cache->LightDiffuse.SetG(g); };
    callbacks.SetLightDiffuseB =    [&](float b){ m_cache->LightDiffuse.SetB(b); };
    callbacks.SetLightSpecularR =   [&](float r){ m_cache->LightSpecular.SetR(r); };
    callbacks.SetLightSpecularG =   [&](float g){ m_cache->LightSpecular.SetG(g); };
    callbacks.SetLightSpecularB =   [&](float b){ m_cache->LightSpecular.SetB(b); };    
    callbacks.SetLightSpecularity = [&](float s){ m_cache->LightSpecularity.Set(s); };  

    tweaker.SetSignalCallbacks(callbacks);
}

void Gui::UpdateCache(Tweaker& tweaker, Editor& editor)
{
    const auto page = ConvertStringToPage(tweaker.GetSelectedPage());
    if(page != m_page)
    {
        m_page = page;
        m_cache->SelectedPage.Set(page);
    }

    if(page == SCENE)
    {
        tweaker.SetDeltaTime(boost::lexical_cast<std::string>(m_cache->DeltaTime.Get()));
        tweaker.SetFramesPerSec(boost::lexical_cast<std::string>(m_cache->FramesPerSec.Get()));

        const Float2 mousePosition = m_cache->MousePosition.Get();
        const Float2 mouseDirection = m_cache->MouseDirection.Get();

        tweaker.SetMousePosition(
            boost::lexical_cast<std::string>(static_cast<int>(mousePosition.x)),
            boost::lexical_cast<std::string>(static_cast<int>(mousePosition.y)));

        tweaker.SetMouseDirection(
            boost::lexical_cast<std::string>(mouseDirection.x),
            boost::lexical_cast<std::string>(mouseDirection.y));
    }
    else if(page == LIGHT)
    {
        if(!tweaker.LightPositionSet() && m_cache->LightPosition.Initialised())
        {
            const Float3 position = m_cache->LightPosition.Get();
            tweaker.SetLightPosition(position.x, position.y, position.z);
        }

        if(!tweaker.LightAttenuationSet() && m_cache->LightAttenuation.Initialised())
        {
            const Float3 attenuation = m_cache->LightAttenuation.Get();
            tweaker.SetLightAttenuation(attenuation.x, attenuation.y, attenuation.z);
        }

        if(!tweaker.LightDiffuseSet() && m_cache->LightDiffuse.Initialised())
        {
            const Colour diffuse = m_cache->LightDiffuse.Get();
            tweaker.SetLightDiffuse(diffuse.r, diffuse.g, diffuse.b);
        }

        if(!tweaker.LightSpecularSet() && m_cache->LightSpecular.Initialised())
        {
            const Colour specular = m_cache->LightSpecular.Get();
            tweaker.SetLightSpecular(specular.r, specular.g, specular.b);
        }

        if(!tweaker.LightSpecularitySet() && m_cache->LightSpecularity.Initialised())
        {
            tweaker.SetLightSpecularity(m_cache->LightSpecularity.Get());
        }
    }
}
