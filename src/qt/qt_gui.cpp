////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "qt/qt_gui.h"
#include "qt/editor.h"
#include "qt/tweaker.h"
#include "qt/qt_gui_reloader.h"

#include "boost/lexical_cast.hpp"

#include <QGuiApplication>
#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQuickStyle>

QtGui::~QtGui() = default;

QtGui::QtGui(std::shared_ptr<Cache> cache)
    : m_cache(cache)
{
}

void QtGui::Run(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");

    QQuickStyle::setStyle("Fusion");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

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

    for (int i = 0; i < TEXTURE_ATTRIBUTES; ++i)
    {
        callbacks.SetTexture[i] = [this, i](float value)
        { 
            m_cache->Texture[i].Set(value);
            m_cache->ReloadTexture.Set(true);
        };
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

    for (int i = 0; i < POST_ATTRIBUTES; ++i)
    {
        callbacks.SetPost[i] = 
            [this, i](float value){ m_cache->Post[i].Set(value); };
    }

    for (int i = 0; i < TERRAIN_ATTRIBUTES; ++i)
    {
        if (i == TERRAIN_MAX_HEIGHT || i == TERRAIN_MIN_HEIGHT || i == TERRAIN_SCALE)
        {
            callbacks.SetTerrain[i] = [this, i](float value)
            { 
                m_cache->Terrain[i].Set(value); 
                m_cache->ReloadTerrain.Set(true);
            };
        }
        else
        {
            callbacks.SetTerrain[i] = 
                [this, i](float value){ m_cache->Terrain[i].Set(value); };
        }
    }

    callbacks.SetSelectedWave =    [this](float index){ m_cache->WaveSelected.Set(static_cast<int>(index)); };
    callbacks.SetSelectedEngine =  [this](int index){ m_cache->EngineSelected.Set(index); };
    callbacks.SetSelectedMesh =    [this](int index){ m_cache->MeshSelected.Set(index); };
    callbacks.SetSelectedWater =   [this](int index){ m_cache->WaterSelected.Set(index); };
    callbacks.SetSelectedLight =   [this](int index){ m_cache->LightSelected.Set(index); };
    callbacks.SetSelectedShader =  [this](int index){ m_cache->ShaderSelected.Set(index); };
    callbacks.SetSelectedTexture = [this](int index){ m_cache->TextureSelected.Set(index); };
    callbacks.SetSelectedEmitter = [this](int index){ m_cache->EmitterSelected.Set(index); };
    callbacks.SetSelectedTerrain = [this](int index){ m_cache->TerrainSelected.Set(index); };
    callbacks.SetPostMap =         [this](int index){ m_cache->PostMapSelected.Set(index); };
    callbacks.ReloadScene =        [this](){ m_cache->ReloadScene.Set(true); };
    callbacks.ReloadEngine =       [this](){ m_cache->ReloadEngine.Set(true); };
    callbacks.ReloadTerrain =      [this](){ m_cache->ReloadTerrain.Set(true); };
    callbacks.ReloadTexture =      [this](){ m_cache->ReloadTexture.Set(true); };
    callbacks.ReloadPlacement =    [this](){ m_cache->ReloadPlacement.Set(true); };
    callbacks.ToggleWireframe =    [this](){ m_cache->ToggleWireframe.Set(true); };
    callbacks.PauseEmission =      [this](){ m_cache->PauseEmission.Set(true); };
    callbacks.RenderLightsOnly =   [this](){ m_cache->RenderLightsOnly.Set(true); };
    callbacks.LightDiagnostics =   [this](){ m_cache->LightDiagnostics.Set(true); };
    callbacks.CompileShader =      [this](const std::string& text){ m_cache->CompileShader.Set(text); };

    QtGuiReloader reloader(engine);
    Tweaker tweaker(callbacks);
    Editor editor(callbacks);

    if (auto context = engine.rootContext())
    {
        context->setContextProperty("Tweaker", &tweaker);
        context->setContextProperty("Editor", &editor);
        context->setContextProperty("Reloader", &reloader);
    }

    engine.load(QUrl("qrc:/TweakerWindow.qml"));
    engine.load(QUrl("qrc:/EditorWindow.qml"));

    QTimer timer;
    timer.setInterval(10);
    QObject::connect(&timer, &QTimer::timeout, &app, [this, &tweaker, &editor, &app]()
    { 
        UpdateTweaker(tweaker);
        UpdateEditor(editor);

        if (!m_cache->ApplicationRunning.Get())
        {
            app.exit();
        }
    });

    timer.start();
    app.exec();
}

void QtGui::UpdateTweaker(Tweaker& tweaker)
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
    case PAGE_AREA:
        UpdateTextures(tweaker);
        UpdateTerrain(tweaker);
        break;
    case PAGE_MESH:
        UpdateMesh(tweaker);
        UpdateWater(tweaker);
        UpdateEmitter(tweaker);
        break;
    case PAGE_POST:
        UpdatePost(tweaker);
        UpdateLight(tweaker);
        break;
    }
}

void QtGui::UpdateEditor(Editor& editor)
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

GuiPage QtGui::ConvertStringToPage(const std::string& page)
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
    else if(page == "Area")
    {
        return PAGE_AREA;
    }
    return PAGE_NONE;
}

void QtGui::UpdatePost(Tweaker& tweaker)
{
    if (!tweaker.HasPostMaps())
    {
        tweaker.InitialisePostMaps(
            m_cache->PostMapSelected.Get(), m_cache->PostMaps.Get());
    }
    else if (m_cache->PostMapSelected.RequiresUpdate())
    {
        tweaker.SetSelectedPostMap(m_cache->PostMapSelected.GetUpdated());
    }

    for (int i = 0; i < POST_ATTRIBUTES; ++i)
    {
        if (m_cache->Post[i].RequiresUpdate())
        {
            tweaker.SetPost(static_cast<PostAttribute>(i), 
                m_cache->Post[i].GetUpdated());
        }
    }
}

void QtGui::UpdateScene(Tweaker& tweaker)
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

    const float deltaTime = m_cache->DeltaTime.Get();
    const float timer = m_cache->Timer.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    tweaker.SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
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

void QtGui::UpdateTerrain(Tweaker& tweaker)
{
    bool initialisedTerrain = false;
    if(!tweaker.HasTerrain())
    {
        initialisedTerrain = true;
        tweaker.InitialiseTerrain(
            m_cache->TerrainSelected.Get(), m_cache->Terrains.Get());
    }

    if (initialisedTerrain || m_cache->TerrainShader.RequiresUpdate())
    {
        tweaker.SetTerrainShaderName(m_cache->TerrainShader.GetUpdated());
    }

    for (int i = 0; i < TERRAIN_ATTRIBUTES; ++i)
    {
        if (initialisedTerrain || m_cache->Terrain[i].RequiresUpdate())
        {
            tweaker.SetTerrain(static_cast<TerrainAttribute>(i), 
                m_cache->Terrain[i].GetUpdated());
        }
    }

    tweaker.SetTerrainInstanceCount(m_cache->TerrainInstances.GetUpdated());
}

void QtGui::UpdateTextures(Tweaker& tweaker)
{
    bool initialisedTextures = false;
    if(!tweaker.HasTextures())
    {
        initialisedTextures = true;
        tweaker.InitialiseTextures(
            m_cache->TextureSelected.Get(), m_cache->Textures.Get());
    }

    for (int i = 0; i < TEXTURE_ATTRIBUTES; ++i)
    {
        if (initialisedTextures || m_cache->Texture[i].RequiresUpdate())
        {
            tweaker.SetTexture(static_cast<TextureAttribute>(i),
                m_cache->Texture[i].GetUpdated());
        }
    }

    if (initialisedTextures || m_cache->TexturePath.RequiresUpdate())
    {
        tweaker.SetTexturePath(m_cache->TexturePath.GetUpdated());
    }
}

void QtGui::UpdateLight(Tweaker& tweaker)
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

void QtGui::UpdateMesh(Tweaker& tweaker)
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

    tweaker.SetMeshInstanceCount(m_cache->MeshInstances.GetUpdated());
}

void QtGui::UpdateEmitter(Tweaker& tweaker)
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

    tweaker.SetEmitterInstanceCount(m_cache->EmitterInstances.GetUpdated());
}

void QtGui::UpdateWater(Tweaker& tweaker)
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

    tweaker.SetWaterInstanceCount(m_cache->WaterInstances.GetUpdated());
}