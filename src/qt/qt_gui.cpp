////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "logger.h"

#include "qt/qt_gui.h"
#include "qt/editor_model.h"
#include "qt/tweaker_model.h"
#include "qt/qt_reloader.h"
#include "qt/attribute_model.h"

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
    m_reloader = std::make_unique<QtReloader>(*m_engine);
    m_tweaker = std::make_unique<TweakerModel>();
    m_editor = std::make_unique<EditorModel>();

    SetupConnections();

    auto context = m_engine->rootContext();
    context->setContextProperty("TweakerModel", m_tweaker.get());
    context->setContextProperty("EditorModel", m_editor.get());
    context->setContextProperty("Reloader", m_reloader.get());

    m_engine->load(QUrl("qrc:/TweakerWindow.qml"));
    //m_engine->load(QUrl("qrc:/EditorWindow.qml"));

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
    const auto page = m_tweaker->SelectedPage();
    if(page != m_page)
    {
        m_page = page;
        m_cache->PageSelected.Set(page);
    }

    switch(page)
    {
    case Tweakable::GuiPage::Scene:
        UpdateScene();
        break;
    case Tweakable::GuiPage::Area:
        UpdateTextures();
        UpdateTerrain();
        break;
    case Tweakable::GuiPage::Mesh:
        UpdateMesh();
        UpdateWater();
        UpdateEmitter();
        break;
    case Tweakable::GuiPage::Post:
        UpdatePost();
        UpdateLight();
        break;
    }
}

void QtGui::UpdateEditor()
{
    bool initialisedShaders = false;
    if(m_editor->Shaders().empty())
    {
        initialisedShaders = true;
        m_editor->InitialiseShaders(
            m_cache->ShaderSelected.Get(), m_cache->Shaders.Get());
    }

    if(initialisedShaders || m_cache->ShaderText.RequiresUpdate())
    {
        m_editor->SetShaderText(QString::fromStdString(m_cache->ShaderText.GetUpdated()));
    }

    if(initialisedShaders || m_cache->ShaderAsm.RequiresUpdate())
    {
        m_editor->SetShaderAssembly(QString::fromStdString(m_cache->ShaderAsm.GetUpdated()));
    }
}

void QtGui::UpdatePost()
{
    if (m_tweaker->PostMaps().empty())
    {
        m_tweaker->InitialisePostMaps(
            m_cache->PostMapSelected.Get(), m_cache->PostMaps.Get());
    }
    else if (m_cache->PostMapSelected.RequiresUpdate())
    {
        m_tweaker->SetPostMapIndex(m_cache->PostMapSelected.GetUpdated());
    }

    auto model = m_tweaker->PostAttributeModel();
    for (int i = 0; i < Tweakable::Post::Max; ++i)
    {
        if (m_cache->Post[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Post[i].GetUpdated());
        }
    }
}

void QtGui::UpdateScene()
{
    if(m_tweaker->Engines().empty())
    {
        m_tweaker->InitialiseEngines(
            m_cache->EngineSelected.Get(), m_cache->Engines.Get());
    }
    else if(m_cache->EngineSelected.RequiresUpdate())
    {
        m_tweaker->SetEngineIndex(m_cache->EngineSelected.GetUpdated());
    }

    const float deltaTime = m_cache->DeltaTime.Get();
    const float timer = m_cache->Timer.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();

    m_tweaker->SetDeltaTime(boost::lexical_cast<std::string>(deltaTime));
    m_tweaker->SetFramesPerSec(boost::lexical_cast<std::string>(framesPerSec));

    auto model = m_tweaker->CameraAttributeModel();
    for (int i = 0; i < Tweakable::Camera::Max; ++i)
    {
        if (m_cache->Camera[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Camera[i].GetUpdated());
        }
    }
}

void QtGui::UpdateTerrain()
{
    bool initialisedTerrain = false;
    if(m_tweaker->Terrain().empty())
    {
        initialisedTerrain = true;
        m_tweaker->InitialiseTerrain(
            m_cache->TerrainSelected.Get(), m_cache->Terrains.Get());
    }

    if (initialisedTerrain || m_cache->TerrainShader.RequiresUpdate())
    {
        m_tweaker->SetTerrainShaderName(m_cache->TerrainShader.GetUpdated());
    }

    auto model = m_tweaker->TerrainAttributeModel();
    for (int i = 0; i < Tweakable::Terrain::Max; ++i)
    {
        if (initialisedTerrain || m_cache->Terrain[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Terrain[i].GetUpdated());
        }
    }

    m_tweaker->SetTerrainInstanceCount(m_cache->TerrainInstances.GetUpdated());
}

void QtGui::UpdateTextures()
{
    bool initialisedTextures = false;
    if(m_tweaker->Textures().empty())
    {
        initialisedTextures = true;
        m_tweaker->InitialiseTextures(
            m_cache->TextureSelected.Get(), m_cache->Textures.Get());
    }

    auto model = m_tweaker->TextureAttributeModel();
    for (int i = 0; i < Tweakable::Texture::Max; ++i)
    {
        if (initialisedTextures || m_cache->Texture[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Texture[i].GetUpdated());
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
    if(m_tweaker->Lights().empty())
    {
        initialisedLights = true;
        m_tweaker->InitialiseLights(
            m_cache->LightSelected.Get(), m_cache->Lights.Get());
    }

    auto model = m_tweaker->LightAttributeModel();
    for (int i = 0; i < Tweakable::Light::Max; ++i)
    {
        if (initialisedLights || m_cache->Light[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Light[i].GetUpdated());
        }
    }
}

void QtGui::UpdateMesh()
{
    bool initialisedMeshes = false;
    if (m_tweaker->Meshes().empty())
    {
        initialisedMeshes = true;
        m_tweaker->InitialiseMeshes(
            m_cache->MeshSelected.Get(), m_cache->Meshes.Get());
    }

    auto model = m_tweaker->MeshAttributeModel();
    for (int i = 0; i < Tweakable::Mesh::Max; ++i)
    {
        if (initialisedMeshes || m_cache->Mesh[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Mesh[i].GetUpdated());
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
    if(m_tweaker->Emitters().empty())
    {
        initialisedEmitter = true;
        m_tweaker->InitialiseEmitters(
            m_cache->EmitterSelected.Get(), m_cache->Emitters.Get());
    }

    auto model = m_tweaker->EmitterAttributeModel();
    for (int i = 0; i < Tweakable::Emitter::Max; ++i)
    {
        if (initialisedEmitter || m_cache->Emitter[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Emitter[i].GetUpdated());
        }
    }

    m_tweaker->SetEmitterInstanceCount(m_cache->EmitterInstances.GetUpdated());
}

void QtGui::UpdateWater()
{
    bool initialisedWater = false;
    if(m_tweaker->Water().empty())
    {
        initialisedWater = true;
        m_tweaker->InitialiseWater(
            m_cache->WaterSelected.Get(), m_cache->Waters.Get());
    }

    auto model = m_tweaker->WaterAttributeModel();
    for (int i = 0; i < Tweakable::Water::Max; ++i)
    {
        if (initialisedWater || m_cache->Water[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Water[i].GetUpdated());
        }
    }

    if(initialisedWater || m_cache->WaveAmount.RequiresUpdate())
    {
        m_tweaker->SetWaveAmount(m_cache->WaveAmount.GetUpdated());
    }

    model = m_tweaker->WaveAttributeModel();
    for (int i = 0; i < Tweakable::Wave::Max; ++i)
    {
        if (initialisedWater || m_cache->Wave[i].RequiresUpdate())
        {
            model->SetAttributeValue(i, m_cache->Wave[i].GetUpdated());
        }
    }

    m_tweaker->SetWaterInstanceCount(m_cache->WaterInstances.GetUpdated());
}

void QtGui::SetupConnections()
{
    connect(m_editor.get(), &EditorModel::RequestCompileSelectedShader, this,
        [this](const QString& text) { m_cache->CompileShader.Set(text.toStdString()); });
    connect(m_editor.get(), &EditorModel::ShaderIndexChanged, this,
        [this](int index) { m_cache->ShaderSelected.Set(index); });

    connect(m_tweaker.get(), &TweakerModel::MeshIndexChanged, this,
        [this](int index) { m_cache->MeshSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::WaveIndexChanged, this,
        [this](int index) { m_cache->WaveSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::EngineIndexChanged, this,
        [this](int index) { m_cache->EngineSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::WaterIndexChanged, this,
        [this](int index) { m_cache->WaterSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::LightIndexChanged, this,
        [this](int index) { m_cache->LightSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::TextureIndexChanged, this,
        [this](int index) { m_cache->TextureSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::EmitterIndexChanged, this,
        [this](int index) { m_cache->EmitterSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::TerrainIndexChanged, this,
        [this](int index) { m_cache->TerrainSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::PostMapIndexChanged, this,
        [this](int index) { m_cache->PostMapSelected.Set(index); });
    connect(m_tweaker.get(), &TweakerModel::RequestReloadScene, this,
        [this]() { m_cache->ReloadScene.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestReloadEngine, this,
        [this]() { m_cache->ReloadEngine.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestReloadTerrain, this,
        [this]() { m_cache->ReloadTerrain.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestReloadTexture, this,
        [this]() { m_cache->ReloadTexture.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestReloadPlacement, this,
        [this]() { m_cache->ReloadPlacement.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestToggleWireframe, this,
        [this]() { m_cache->ToggleWireframe.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestTogglePauseEmission, this,
        [this]() { m_cache->PauseEmission.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestToggleLightsOnly, this,
        [this]() { m_cache->RenderLightsOnly.Set(true); });
    connect(m_tweaker.get(), &TweakerModel::RequestToggleLightsDiagnostics, this,
        [this]() { m_cache->LightDiagnostics.Set(true); });

    for (int i = 0; i < Tweakable::Light::Max; ++i)
    {
        connect(m_tweaker->LightAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {  
                m_cache->Light[attribute].Set(value); 
            });
    }

    for (int i = 0; i < Tweakable::Camera::Max; ++i)
    {
        connect(m_tweaker->CameraAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Camera[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Mesh::Max; ++i)
    {
        connect(m_tweaker->MeshAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Mesh[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Texture::Max; ++i)
    {
        connect(m_tweaker->TextureAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Texture[attribute].Set(value);
                m_cache->ReloadTexture.Set(true);
            });
    }

    for (int i = 0; i < Tweakable::Water::Max; ++i)
    {
        connect(m_tweaker->WaterAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Water[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Wave::Max; ++i)
    {
        connect(m_tweaker->WaveAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Wave[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Emitter::Max; ++i)
    {
        connect(m_tweaker->EmitterAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Emitter[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Post::Max; ++i)
    {
        connect(m_tweaker->PostAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Post[attribute].Set(value);
            });
    }

    for (int i = 0; i < Tweakable::Terrain::Max; ++i)
    {
        connect(m_tweaker->TerrainAttributeModel(), &AttributeModel::AttributeValueChanged, this,
            [this](int attribute, float value) {
                m_cache->Terrain[attribute].Set(value);
                if (attribute == Tweakable::Terrain::MaxHeight ||
                    attribute == Tweakable::Terrain::MinHeight ||
                    attribute == Tweakable::Terrain::Scale)
                {
                    m_cache->ReloadTerrain.Set(true);
                }
            });
    }
}