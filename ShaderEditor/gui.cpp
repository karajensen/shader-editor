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
    callbacks.SetLightSpecularity = [&](float value){ m_cache->LightSpecularity.Set(value); };  
    callbacks.SetDOFDistance =      [&](float value){ m_cache->DOFDistance.Set(value); };
    callbacks.SetDOFFade =          [&](float value){ m_cache->DOFFade.Set(value); };
    callbacks.SetBlurAmount =       [&](float value){ m_cache->BlurAmount.Set(value); };
    callbacks.SetBlurStep =         [&](float value){ m_cache->BlurStep.Set(value); };
    callbacks.SetGlowAmount =       [&](float value){ m_cache->GlowAmount.Set(value); };
    callbacks.SetDepthFar =         [&](float value){ m_cache->DepthFar.Set(value); };
    callbacks.SetDepthNear =        [&](float value){ m_cache->DepthNear.Set(value); };
    callbacks.SetMeshSpecularity =  [&](float value){ m_cache->MeshSpecularity.Set(value); };  
    callbacks.SetMeshAmbience =     [&](float value){ m_cache->MeshAmbience.Set(value); };
    callbacks.SetMeshBump =         [&](float value){ m_cache->MeshBump.Set(value); };
    callbacks.SetMeshGlow =         [&](float value){ m_cache->MeshGlow.Set(value); };
    callbacks.SetMaxRed =           [&](float value){ m_cache->MaximumColour.SetR(value); };
    callbacks.SetMaxGreen =         [&](float value){ m_cache->MaximumColour.SetG(value); };
    callbacks.SetMaxBlue =          [&](float value){ m_cache->MaximumColour.SetB(value); };
    callbacks.SetMinRed =           [&](float value){ m_cache->MinimumColour.SetR(value); };
    callbacks.SetMinGreen =         [&](float value){ m_cache->MinimumColour.SetG(value); };
    callbacks.SetMinBlue =          [&](float value){ m_cache->MinimumColour.SetB(value); };
    callbacks.SetSelectedEngine =   [&](int index){ m_cache->EngineSelected.Set(index); };
    callbacks.SetSelectedMesh =     [&](int index){ m_cache->MeshSelected.Set(index); };
    callbacks.SetSelectedLight =    [&](int index){ m_cache->LightSelected.Set(index); };
    callbacks.SetSelectedShader =   [&](int index){ m_cache->ShaderSelected.Set(index); };
    callbacks.SetPostMap =          [&](int index){ m_cache->PostMapSelected.Set(index); };
    callbacks.CompileShader =       [&](const std::string& text){ m_cache->CompileShader.Set(text); };

    Editor editor(callbacks);
    editor.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    editor.show();

    Tweaker tweaker(callbacks);
    tweaker.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    tweaker.show();

    while(m_cache->ApplicationRunning.Get())
    {
        app.processEvents();
        UpdateTweaker(tweaker);
        UpdateEditor(editor);
    }

    app.exit();
    Logger::LogInfo("Exiting Qt");
}

void Gui::UpdateTweaker(Tweaker& tweaker)
{
    const auto page = ConvertStringToPage(tweaker.GetSelectedPage());
    if(page != m_page)
    {
        m_page = page;
        m_cache->PageSelected.Set(page);
    }

    switch(page)
    {
    case SCENE:
        UpdateScene(tweaker);
        break;
    case LIGHT:
        UpdateLight(tweaker);
        break;
    case MESH:
        UpdateMesh(tweaker);
        break;
    case POST:
        UpdatePost(tweaker);
        break;
    }
}

void Gui::UpdateEditor(Editor& editor)
{
    bool initialisedShaders = false;
    if(!editor.HasShaders())
    {
        initialisedShaders = true;
        editor.InitialiseShaders(
            m_cache->ShaderSelected.Get(), m_cache->Shaders.Get());
    }

    if(initialisedShaders || m_cache->ShaderText.RequiresUpdate())
    {
        editor.SetShaderText(m_cache->ShaderText.GetUpdated());
    }

    if(initialisedShaders || m_cache->ShaderAsm.RequiresUpdate())
    {
        editor.SetShaderAssembly(m_cache->ShaderAsm.GetUpdated());
    }
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

void Gui::UpdatePost(Tweaker& tweaker)
{
    if (m_cache->DOFDistance.RequiresUpdate())
    {
        tweaker.SetDOFDistance(m_cache->DOFDistance.GetUpdated());
    }

    if (m_cache->DOFFade.RequiresUpdate())
    {
        tweaker.SetDOFFade(m_cache->DOFFade.GetUpdated());
    }

    if (m_cache->BlurAmount.RequiresUpdate())
    {
        tweaker.SetBlurAmount(m_cache->BlurAmount.GetUpdated());
    }

    if (m_cache->BlurStep.RequiresUpdate())
    {
        tweaker.SetBlurStep(m_cache->BlurStep.GetUpdated());
    }

    if (m_cache->GlowAmount.RequiresUpdate())
    {
        tweaker.SetGlowAmount(m_cache->GlowAmount.GetUpdated());
    }

    if (m_cache->DepthFar.RequiresUpdate())
    {
        tweaker.SetDepthFar(m_cache->DepthFar.GetUpdated());
    }

    if (m_cache->DepthNear.RequiresUpdate())
    {
        tweaker.SetDepthNear(m_cache->DepthNear.GetUpdated());
    }

    if (m_cache->MinimumColour.RequiresUpdate())
    {
        const Colour colour = m_cache->MinimumColour.GetUpdated();
        tweaker.SetMinimumColour(colour.r, colour.g, colour.b);
    }

    if (m_cache->MaximumColour.RequiresUpdate())
    {
        const Colour colour = m_cache->MaximumColour.GetUpdated();
        tweaker.SetMaximumColour(colour.r, colour.g, colour.b);
    }
}

void Gui::UpdateScene(Tweaker& tweaker)
{
    if(!tweaker.HasEngines())
    {
        tweaker.InitialiseEngines(
            m_cache->EngineSelected.Get(), m_cache->Engines.Get());
    }
    else if(m_cache->EngineSelected.RequiresUpdate())
    {
        tweaker.SetSelectedEngine(m_cache->EngineSelected.GetUpdated());
    }

    if (!tweaker.HasPostMaps())
    {
        tweaker.InitialisePostMaps(
            m_cache->PostMapSelected.Get(), m_cache->PostMaps.Get());
    }
    else if (m_cache->PostMapSelected.RequiresUpdate())
    {
        tweaker.SetSelectedPostMap(m_cache->PostMapSelected.GetUpdated());
    }

    const Float2 mousePosition = m_cache->MousePosition.Get();
    const Float2 mouseDirection = m_cache->MouseDirection.Get();
    const float deltaTime = m_cache->DeltaTime.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    tweaker.SetMousePosition(
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.x)),
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.y)));

    tweaker.SetMouseDirection(
        boost::lexical_cast<std::string>(mouseDirection.x),
        boost::lexical_cast<std::string>(mouseDirection.y));

    tweaker.SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    tweaker.SetFramesPerSec(boost::lexical_cast<std::string>(framesPerSec));
}

void Gui::UpdateLight(Tweaker& tweaker)
{
    bool initialisedLights = false;
    if(!tweaker.HasLights())
    {
        initialisedLights = true;
        tweaker.InitialiseLights(
            m_cache->LightSelected.Get(), m_cache->Lights.Get());
    }

    if(initialisedLights || m_cache->LightPosition.RequiresUpdate())
    {
        const Float3 position = m_cache->LightPosition.GetUpdated();
        tweaker.SetLightPosition(position.x, position.y, position.z);
    }

    if(initialisedLights || m_cache->LightAttenuation.RequiresUpdate())
    {
        const Float3 attenuation = m_cache->LightAttenuation.GetUpdated();
        tweaker.SetLightAttenuation(attenuation.x, attenuation.y, attenuation.z);
    }

    if(initialisedLights || m_cache->LightSpecular.RequiresUpdate())
    {
        const Colour specular = m_cache->LightSpecular.GetUpdated();
        tweaker.SetLightSpecular(specular.r, specular.g, specular.b);
    }

    if(initialisedLights || m_cache->LightDiffuse.RequiresUpdate())
    {
        const Colour diffuse = m_cache->LightDiffuse.GetUpdated();
        tweaker.SetLightDiffuse(diffuse.r, diffuse.g, diffuse.b);
    }

    if(initialisedLights || m_cache->LightSpecularity.RequiresUpdate())
    {
        tweaker.SetLightSpecularity(m_cache->LightSpecularity.GetUpdated());
    }
}

void Gui::UpdateMesh(Tweaker& tweaker)
{
    bool initialisedMeshes = false;
    if(!tweaker.HasMeshes())
    {
        initialisedMeshes = true;
        tweaker.InitialiseMeshes(
            m_cache->MeshSelected.Get(), m_cache->Meshes.Get());
    }

    if(initialisedMeshes || m_cache->MeshSpecularity.RequiresUpdate())
    {
        tweaker.SetMeshSpecularity(m_cache->MeshSpecularity.GetUpdated());
    }

    if(initialisedMeshes || m_cache->MeshAmbience.RequiresUpdate())
    {
        tweaker.SetMeshAmbience(m_cache->MeshAmbience.GetUpdated());
    }

    if(initialisedMeshes || m_cache->MeshBump.RequiresUpdate())
    {
        tweaker.SetMeshBump(m_cache->MeshBump.GetUpdated());
    }

    if(initialisedMeshes || m_cache->MeshGlow.RequiresUpdate())
    {
        tweaker.SetMeshGlow(m_cache->MeshGlow.GetUpdated());
    }

    if(initialisedMeshes || m_cache->MeshShader.RequiresUpdate())
    {
        tweaker.SetMeshShaderName(m_cache->MeshShader.GetUpdated());
    }
}
