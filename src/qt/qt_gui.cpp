////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "logger.h"

#include "qt/qt_gui.h"
#include "qt/editor_model.h"
#include "qt/tweaker_model.h"
#include "qt/qt_reloader.h"

#include <boost/lexical_cast.hpp>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQuickStyle>

QtGui::~QtGui() = default;

QtGui::QtGui(std::shared_ptr<Cache> cache)
    : QObject(nullptr)
    , m_cache(cache)
{
}

void QtGui::Run(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");

    QQuickStyle::setStyle("Fusion");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    m_app = std::make_unique<QGuiApplication>(argc, argv);
    m_engine = std::make_unique<QQmlApplicationEngine>();

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

    m_reloader = std::make_unique<QtReloader>(*m_engine);
    m_tweaker = std::make_unique<TweakerModel>(callbacks);
    m_editor = std::make_unique<EditorModel>(callbacks);

    if (auto context = m_engine->rootContext())
    {
        context->setContextProperty("TweakerModel", m_tweaker.get());
        context->setContextProperty("EditorModel", m_editor.get());
        context->setContextProperty("Reloader", m_reloader.get());
    }

    //engine.load(QUrl("qrc:/TweakerWindow.qml"));
    m_engine->load(QUrl("qrc:/EditorWindow.qml"));

    QTimer timer; 
    timer.setInterval(10);
    QObject::connect(&timer, &QTimer::timeout, this, [this]()
    { 
        UpdateTweaker();
        UpdateEditor();

        if (!m_cache->ApplicationRunning.Get())
        {
            m_engine.reset();
            m_app->exit();
        }
    });

    timer.start();
    m_app->exec();
}

void QtGui::UpdateTweaker()
{
    const auto page = ConvertStringToPage(m_tweaker->GetSelectedPage());
    if(page != m_page)
    {
        m_page = page;
        m_cache->PageSelected.Set(page);
    }

    switch(page)
    {
    case PAGE_SCENE:
        UpdateScene();
        break;
    case PAGE_AREA:
        UpdateTextures();
        UpdateTerrain();
        break;
    case PAGE_MESH:
        UpdateMesh();
        UpdateWater();
        UpdateEmitter();
        break;
    case PAGE_POST:
        UpdatePost();
        UpdateLight();
        break;
    }
}

void QtGui::UpdateEditor()
{
    bool initialisedShaders = false;
    if(!m_editor->HasShaders())
    {
        initialisedShaders = true;
        m_editor->InitialiseShaders(
            m_cache->ShaderSelected.Get(), m_cache->Shaders.Get());
    }

    if(initialisedShaders || m_cache->ShaderText.RequiresUpdate())
    {
        m_editor->SetShaderText(m_cache->ShaderText.GetUpdated());
    }

    if(initialisedShaders || m_cache->ShaderAsm.RequiresUpdate())
    {
        m_editor->SetShaderAssembly(m_cache->ShaderAsm.GetUpdated());
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

void QtGui::UpdatePost()
{
    if (!m_tweaker->HasPostMaps())
    {
        m_tweaker->InitialisePostMaps(
            m_cache->PostMapSelected.Get(), m_cache->PostMaps.Get());
    }
    else if (m_cache->PostMapSelected.RequiresUpdate())
    {
        m_tweaker->SetSelectedPostMap(m_cache->PostMapSelected.GetUpdated());
    }

    for (int i = 0; i < POST_ATTRIBUTES; ++i)
    {
        if (m_cache->Post[i].RequiresUpdate())
        {
            m_tweaker->SetPost(static_cast<PostAttribute>(i), 
                m_cache->Post[i].GetUpdated());
        }
    }
}

void QtGui::UpdateScene()
{
    if(!m_tweaker->HasEngines())
    {
        m_tweaker->InitialiseEngines(
            m_cache->EngineSelected.Get(), m_cache->Engines.Get());
    }
    else if(m_cache->EngineSelected.RequiresUpdate())
    {
        m_tweaker->SetSelectedEngine(m_cache->EngineSelected.GetUpdated());
    }

    const float deltaTime = m_cache->DeltaTime.Get();
    const float timer = m_cache->Timer.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    m_tweaker->SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    m_tweaker->SetFramesPerSec(boost::lexical_cast<std::string>(framesPerSec));

    for (int i = 0; i < CAMERA_ATTRIBUTES; ++i)
    {
        if (m_cache->Camera[i].RequiresUpdate())
        {
            m_tweaker->SetCamera(static_cast<CameraAttribute>(i), 
                m_cache->Camera[i].GetUpdated());
        }
    }
}

void QtGui::UpdateTerrain()
{
    bool initialisedTerrain = false;
    if(!m_tweaker->HasTerrain())
    {
        initialisedTerrain = true;
        m_tweaker->InitialiseTerrain(
            m_cache->TerrainSelected.Get(), m_cache->Terrains.Get());
    }

    if (initialisedTerrain || m_cache->TerrainShader.RequiresUpdate())
    {
        m_tweaker->SetTerrainShaderName(m_cache->TerrainShader.GetUpdated());
    }

    for (int i = 0; i < TERRAIN_ATTRIBUTES; ++i)
    {
        if (initialisedTerrain || m_cache->Terrain[i].RequiresUpdate())
        {
            m_tweaker->SetTerrain(static_cast<TerrainAttribute>(i), 
                m_cache->Terrain[i].GetUpdated());
        }
    }

    m_tweaker->SetTerrainInstanceCount(m_cache->TerrainInstances.GetUpdated());
}

void QtGui::UpdateTextures()
{
    bool initialisedTextures = false;
    if(!m_tweaker->HasTextures())
    {
        initialisedTextures = true;
        m_tweaker->InitialiseTextures(
            m_cache->TextureSelected.Get(), m_cache->Textures.Get());
    }

    for (int i = 0; i < TEXTURE_ATTRIBUTES; ++i)
    {
        if (initialisedTextures || m_cache->Texture[i].RequiresUpdate())
        {
            m_tweaker->SetTexture(static_cast<TextureAttribute>(i),
                m_cache->Texture[i].GetUpdated());
        }
    }

    if (initialisedTextures || m_cache->TexturePath.RequiresUpdate())
    {
        m_tweaker->SetTexturePath(m_cache->TexturePath.GetUpdated());
    }
}

void QtGui::UpdateLight()
{
    bool initialisedLights = false;
    if(!m_tweaker->HasLights())
    {
        initialisedLights = true;
        m_tweaker->InitialiseLights(
            m_cache->LightSelected.Get(), m_cache->Lights.Get());
    }

    for (int i = 0; i < LIGHT_ATTRIBUTES; ++i)
    {
        if (initialisedLights || m_cache->Light[i].RequiresUpdate())
        {
            m_tweaker->SetLight(static_cast<LightAttribute>(i), 
                m_cache->Light[i].GetUpdated());
        }
    }
}

void QtGui::UpdateMesh()
{
    bool initialisedMeshes = false;
    if (!m_tweaker->HasMeshes())
    {
        initialisedMeshes = true;
        m_tweaker->InitialiseMeshes(
            m_cache->MeshSelected.Get(), m_cache->Meshes.Get());
    }

    for (int i = 0; i < MESH_ATTRIBUTES; ++i)
    {
        if (initialisedMeshes || m_cache->Mesh[i].RequiresUpdate())
        {
            m_tweaker->SetMesh(static_cast<MeshAttribute>(i),
                m_cache->Mesh[i].GetUpdated());
        }
    }

    if (initialisedMeshes || m_cache->MeshShader.RequiresUpdate())
    {
        m_tweaker->SetMeshShaderName(m_cache->MeshShader.GetUpdated());
    }

    m_tweaker->SetMeshInstanceCount(m_cache->MeshInstances.GetUpdated());
}

void QtGui::UpdateEmitter()
{
    bool initialisedEmitter = false;
    if(!m_tweaker->HasEmitters())
    {
        initialisedEmitter = true;
        m_tweaker->InitialiseEmitters(
            m_cache->EmitterSelected.Get(), m_cache->Emitters.Get());
    }

    for (int i = 0; i < EMITTER_ATTRIBUTES; ++i)
    {
        if (initialisedEmitter || m_cache->Emitter[i].RequiresUpdate())
        {
            m_tweaker->SetEmitter(static_cast<EmitterAttribute>(i), 
                m_cache->Emitter[i].GetUpdated());
        }
    }

    m_tweaker->SetEmitterInstanceCount(m_cache->EmitterInstances.GetUpdated());
}

void QtGui::UpdateWater()
{
    bool initialisedWater = false;
    if(!m_tweaker->HasWater())
    {
        initialisedWater = true;
        m_tweaker->InitialiseWater(
            m_cache->WaterSelected.Get(), m_cache->Waters.Get());
    }

    for (int i = 0; i < WATER_ATTRIBUTES; ++i)
    {
        if (initialisedWater || m_cache->Water[i].RequiresUpdate())
        {
            m_tweaker->SetWater(static_cast<WaterAttribute>(i), 
                m_cache->Water[i].GetUpdated());
        }
    }

    if(initialisedWater || m_cache->WaveAmount.RequiresUpdate())
    {
        m_tweaker->SetWaveAmount(m_cache->WaveAmount.GetUpdated());
    }

    for (int i = 0; i < WAVE_ATTRIBUTES; ++i)
    {
        if (initialisedWater || m_cache->Wave[i].RequiresUpdate())
        {
            m_tweaker->SetWave(static_cast<WaveAttribute>(i), 
                m_cache->Wave[i].GetUpdated());
        }
    }

    m_tweaker->SetWaterInstanceCount(m_cache->WaterInstances.GetUpdated());
}