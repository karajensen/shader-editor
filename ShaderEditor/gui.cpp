////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "qt/project/editor.h"
#include "qt/project/tweaker.h"
#include "qt/include/QtWidgets/qapplication.h"
#include "boost/lexical_cast.hpp"

Gui::~Gui() = default;

Gui::Gui(std::shared_ptr<Cache> cache) :
    m_cache(cache)
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

    for (int i = 0; i < CAMERA_ATTRIBUTES; ++i)
    {
        callbacks.SetCamera[i] = 
            [this, i](float value){ m_cache->Camera[i].Set(value); };
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

    for (int i = 0; i < WAVE_ATTRIBUTES; ++i)
    {
        callbacks.SetWave[i] = 
            [this, i](float value){ m_cache->Wave[i].Set(value); };
    }

    for (int i = 0; i < EMITTER_ATTRIBUTES; ++i)
    {
        callbacks.SetEmitter[i] = 
            [this, i](float value){ m_cache->Emitter[i].Set(value); };
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

    callbacks.SetDOFDistance =     [this](float value){ m_cache->DOFDistance.Set(value); };
    callbacks.SetDOFFade =         [this](float value){ m_cache->DOFFade.Set(value); };
    callbacks.SetBlurAmount =      [this](float value){ m_cache->BlurAmount.Set(value); };
    callbacks.SetBlurStep =        [this](float value){ m_cache->BlurStep.Set(value); };
    callbacks.SetGlowAmount =      [this](float value){ m_cache->GlowAmount.Set(value); };
    callbacks.SetContrast =        [this](float value){ m_cache->Contrast.Set(value); };
    callbacks.SetSaturation =      [this](float value){ m_cache->Saturation.Set(value); };
    callbacks.SetDepthFar =        [this](float value){ m_cache->DepthFar.Set(value); };
    callbacks.SetDepthNear =       [this](float value){ m_cache->DepthNear.Set(value); };
    callbacks.SetParticleAmount =  [this](float index){ m_cache->ParticleAmount.Set(static_cast<int>(index)); };
    callbacks.SetSelectedWave =    [this](float index){ m_cache->WaveSelected.Set(static_cast<int>(index)); };
    callbacks.SetSelectedEngine =  [this](int index){ m_cache->EngineSelected.Set(index); };
    callbacks.SetSelectedMesh =    [this](int index){ m_cache->MeshSelected.Set(index); };
    callbacks.SetSelectedWater =   [this](int index){ m_cache->WaterSelected.Set(index); };
    callbacks.SetSelectedLight =   [this](int index){ m_cache->LightSelected.Set(index); };
    callbacks.SetSelectedShader =  [this](int index){ m_cache->ShaderSelected.Set(index); };
    callbacks.SetSelectedEmitter = [this](int index){ m_cache->EmitterSelected.Set(index); };
    callbacks.SetPostMap =         [this](int index){ m_cache->PostMapSelected.Set(index); };
    callbacks.ReloadScene =        [this](){ m_cache->ReloadScene.Set(true); };
    callbacks.SaveScene =          [this](){ m_cache->SaveScene.Set(true); };
    callbacks.SavePost =           [this](){ m_cache->SavePost.Set(true); };
    callbacks.PauseEmission =      [this](){ m_cache->PauseEmission.Set(true); };
    callbacks.CompileShader =      [this](const std::string& text){ m_cache->CompileShader.Set(text); };

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
    case PAGE_SCENE:
        UpdateScene(tweaker);
        break;
    case PAGE_LIGHT:
        UpdateLight(tweaker);
        break;
    case PAGE_MESH:
        UpdateMesh(tweaker);
        UpdateWater(tweaker);
        UpdateEmitter(tweaker);
        break;
    case PAGE_POST:
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
        return PAGE_SCENE;
    }
    else if(page == "Mesh")
    {
        return PAGE_MESH;
    }
    else if(page == "Post")
    {
        return PAGE_POST;
    }
    else if(page == "Light")
    {
        return PAGE_LIGHT;
    }
    return PAGE_NONE;
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

    const float deltaTime = m_cache->DeltaTime.Get();
    const float timer = m_cache->Timer.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    tweaker.SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    tweaker.SetTimer(boost::lexical_cast<std::string>(timer));
    tweaker.SetFramesPerSec(boost::lexical_cast<std::string>(framesPerSec));

    for (int i = 0; i < CAMERA_ATTRIBUTES; ++i)
    {
        if (m_cache->Camera[i].RequiresUpdate())
        {
            tweaker.SetCamera(static_cast<CameraAttribute>(i), 
                m_cache->Camera[i].GetUpdated());
        }
    }
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
    if (!tweaker.HasMeshes())
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

    if (initialisedMeshes || m_cache->MeshShader.RequiresUpdate())
    {
        tweaker.SetMeshShaderName(m_cache->MeshShader.GetUpdated());
    }
}

void Gui::UpdateEmitter(Tweaker& tweaker)
{
    bool initialisedEmitter = false;
    if(!tweaker.HasEmitters())
    {
        initialisedEmitter = true;
        tweaker.InitialiseEmitters(
            m_cache->EmitterSelected.Get(), m_cache->Emitters.Get());
    }

    for (int i = 0; i < EMITTER_ATTRIBUTES; ++i)
    {
        if (initialisedEmitter || m_cache->Emitter[i].RequiresUpdate())
        {
            tweaker.SetEmitter(static_cast<EmitterAttribute>(i), 
                m_cache->Emitter[i].GetUpdated());
        }
    }

    if(initialisedEmitter || m_cache->ParticleAmount.RequiresUpdate())
    {
        tweaker.SetParticleAmount(m_cache->ParticleAmount.GetUpdated());
    }
}

void Gui::UpdateWater(Tweaker& tweaker)
{
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

    if(initialisedWater || m_cache->WaveAmount.RequiresUpdate())
    {
        tweaker.SetWaveAmount(m_cache->WaveAmount.GetUpdated());
    }

    for (int i = 0; i < WAVE_ATTRIBUTES; ++i)
    {
        if (initialisedWater || m_cache->Wave[i].RequiresUpdate())
        {
            tweaker.SetWave(static_cast<WaveAttribute>(i), 
                m_cache->Wave[i].GetUpdated());
        }
    }
}
