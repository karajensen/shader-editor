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

    for (int i = 0; i < LIGHT_ATTRIBUTES; ++i)
    {
        callbacks.SetLight[i] = 
            [this, i](float value){ m_cache->Light[i].Set(value); };
    }

    for (int i = 0; i < MESH_ATTRIBUTES; ++i)
    {
        callbacks.SetMesh[i] = 
            [this, i](float value){ m_cache->Mesh[i].Set(value); };
    }

    for (int i = 0; i < WATER_ATTRIBUTES; ++i)
    {
        callbacks.SetWater[i] = 
            [this, i](float value){ m_cache->Water[i].Set(value); };
    }

    for (int i = 0; i < FOG_ATTRIBUTES; ++i)
    {
        callbacks.SetFog[i] = 
            [this, i](float value){ m_cache->Fog[i].Set(value); };
    }

    for (int i = 0; i < COLOUR_ATTRIBUTES; ++i)
    {
        callbacks.SetMinColour[i] = 
            [this, i](float value){ m_cache->MinColour[i].Set(value); };

        callbacks.SetMaxColour[i] = 
            [this, i](float value){ m_cache->MaxColour[i].Set(value); };
    }

    callbacks.SetDOFDistance =    [&](float value){ m_cache->DOFDistance.Set(value); };
    callbacks.SetDOFFade =        [&](float value){ m_cache->DOFFade.Set(value); };
    callbacks.SetBlurAmount =     [&](float value){ m_cache->BlurAmount.Set(value); };
    callbacks.SetBlurStep =       [&](float value){ m_cache->BlurStep.Set(value); };
    callbacks.SetGlowAmount =     [&](float value){ m_cache->GlowAmount.Set(value); };
    callbacks.SetContrast =       [&](float value){ m_cache->Contrast.Set(value); };
    callbacks.SetSaturation =     [&](float value){ m_cache->Saturation.Set(value); };
    callbacks.SetDepthFar =       [&](float value){ m_cache->DepthFar.Set(value); };
    callbacks.SetDepthNear =      [&](float value){ m_cache->DepthNear.Set(value); };
    callbacks.SetSelectedEngine = [&](int index){ m_cache->EngineSelected.Set(index); };
    callbacks.SetSelectedMesh =   [&](int index){ m_cache->MeshSelected.Set(index); };
    callbacks.SetSelectedWater =  [&](int index){ m_cache->WaterSelected.Set(index); };
    callbacks.SetSelectedLight =  [&](int index){ m_cache->LightSelected.Set(index); };
    callbacks.SetSelectedShader = [&](int index){ m_cache->ShaderSelected.Set(index); };
    callbacks.SetPostMap =        [&](int index){ m_cache->PostMapSelected.Set(index); };
    callbacks.SaveLights =        [&](){ m_cache->SaveLights.Set(true); };
    callbacks.SaveMeshes =        [&](){ m_cache->SaveMeshes.Set(true); };
    callbacks.SavePost =          [&](){ m_cache->SavePost.Set(true); };
    callbacks.CompileShader =     [&](const std::string& text){ m_cache->CompileShader.Set(text); };

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

    if (m_cache->Saturation.RequiresUpdate())
    {
        tweaker.SetSaturation(m_cache->Saturation.GetUpdated());
    }

    if (m_cache->Contrast.RequiresUpdate())
    {
        tweaker.SetContrast(m_cache->Contrast.GetUpdated());
    }

    if (m_cache->DepthFar.RequiresUpdate())
    {
        tweaker.SetDepthFar(m_cache->DepthFar.GetUpdated());
    }

    if (m_cache->DepthNear.RequiresUpdate())
    {
        tweaker.SetDepthNear(m_cache->DepthNear.GetUpdated());
    }

    for (int i = 0; i < FOG_ATTRIBUTES; ++i)
    {
        if (m_cache->Fog[i].RequiresUpdate())
        {
            tweaker.SetFog(static_cast<FogAttribute>(i), 
                m_cache->Fog[i].GetUpdated());
        }
    }

    for (int i = 0; i < COLOUR_ATTRIBUTES; ++i)
    {
        if (m_cache->MinColour[i].RequiresUpdate())
        {
            tweaker.SetMinimumColour(static_cast<ColourAttribute>(i), 
                m_cache->MinColour[i].GetUpdated());
        }
        if (m_cache->MaxColour[i].RequiresUpdate())
        {
            tweaker.SetMaximumColour(static_cast<ColourAttribute>(i),
                m_cache->MaxColour[i].GetUpdated());
        }
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
    const double timer = m_cache->Timer.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    tweaker.SetMousePosition(
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.x)),
        boost::lexical_cast<std::string>(static_cast<int>(mousePosition.y)));

    tweaker.SetMouseDirection(
        boost::lexical_cast<std::string>(mouseDirection.x),
        boost::lexical_cast<std::string>(mouseDirection.y));

    tweaker.SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    tweaker.SetTimer(boost::lexical_cast<std::string>(timer));
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

    for (int i = 0; i < LIGHT_ATTRIBUTES; ++i)
    {
        if (initialisedLights || m_cache->Light[i].RequiresUpdate())
        {
            tweaker.SetLight(static_cast<LightAttribute>(i), 
                m_cache->Light[i].GetUpdated());
        }
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

    for (int i = 0; i < MESH_ATTRIBUTES; ++i)
    {
        if (initialisedMeshes || m_cache->Mesh[i].RequiresUpdate())
        {
            tweaker.SetMesh(static_cast<MeshAttribute>(i), 
                m_cache->Mesh[i].GetUpdated());
        }
    }

    if(initialisedMeshes || m_cache->MeshShader.RequiresUpdate())
    {
        tweaker.SetMeshShaderName(m_cache->MeshShader.GetUpdated());
    }

    bool initialisedWater = false;
    if(!tweaker.HasWater())
    {
        initialisedWater = true;
        tweaker.InitialiseWater(
            m_cache->WaterSelected.Get(), m_cache->Waters.Get());
    }

    for (int i = 0; i < WATER_ATTRIBUTES; ++i)
    {
        if (initialisedWater || m_cache->Water[i].RequiresUpdate())
        {
            tweaker.SetWater(static_cast<WaterAttribute>(i), 
                m_cache->Water[i].GetUpdated());
        }
    }
}
