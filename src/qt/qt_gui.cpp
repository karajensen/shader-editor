////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "qt/qt_gui.h"
#include "qt/qt_reloader.h"
#include "qt/editor_model.h"
#include "qt/tweaker_model.h"
#include "qt/stringlist_model.h"
#include "qt/attribute_model.h"

#include "logger.h"

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
    QObject::connect(&timer, &QTimer::timeout, this, [this, &timer]()
    { 
        UpdateTweaker();
        UpdateEditor();

        if (!m_cache->ApplicationRunning.Get())
        {
            timer.stop();
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
    if (auto model = m_editor->ShadersModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Shaders.Get());
            model->SetSelectedIndex(m_cache->ShaderSelected.Get());
        }
    }

    if(m_cache->ShaderText.RequiresUpdate())
    {
        m_editor->SetShaderText(QString::fromStdString(m_cache->ShaderText.GetUpdated()));
    }

    if(m_cache->ShaderAsm.RequiresUpdate())
    {
        m_editor->SetShaderAssembly(QString::fromStdString(m_cache->ShaderAsm.GetUpdated()));
    }
}

void QtGui::UpdatePost()
{
    if (auto model = m_tweaker->PostMapsModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->PostMaps.Get());
            model->SetSelectedIndex(m_cache->PostMapSelected.Get());
        }
    }

    if (auto model = m_tweaker->PostAttributeModel())
    {
        for (int i = 0; i < Tweakable::Post::Max; ++i)
        {
            if (m_cache->Post[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Post[i].GetUpdated());
            }
        }
    }
}

void QtGui::UpdateScene()
{
    if (auto model = m_tweaker->EnginesModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Engines.Get());
            model->SetSelectedIndex(m_cache->EngineSelected.Get());
        }
    }

    if (auto model = m_tweaker->CameraAttributeModel())
    {
        for (int i = 0; i < Tweakable::Camera::Max; ++i)
        {
            if (m_cache->Camera[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Camera[i].GetUpdated());
            }
        }
    }

    const float deltaTime = m_cache->DeltaTime.Get();
    const int framesPerSec = m_cache->FramesPerSec.Get();
    m_tweaker->SetDeltaTime(deltaTime);
    m_tweaker->SetFramesPerSecond(framesPerSec);
}

void QtGui::UpdateTerrain()
{
    if (auto model = m_tweaker->TerrainModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Terrains.Get());
            model->SetSelectedIndex(m_cache->TerrainSelected.Get());
        }
    }

    if (auto model = m_tweaker->TerrainAttributeModel())
    {
        for (int i = 0; i < Tweakable::Terrain::Max; ++i)
        {
            if (m_cache->Terrain[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Terrain[i].GetUpdated());
            }
        }
    }

    if (m_cache->TerrainShader.RequiresUpdate())
    {
        m_tweaker->SetTerrainShader(QString::fromStdString(m_cache->TerrainShader.GetUpdated()));
    }

    if (m_cache->TerrainInstances.RequiresUpdate())
    {
        m_tweaker->SetTerrainInstances(QString::fromStdString(m_cache->TerrainInstances.GetUpdated()));
    }
}

void QtGui::UpdateTextures()
{
    if (auto model = m_tweaker->TexturesModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Textures.Get());
            model->SetSelectedIndex(m_cache->TextureSelected.Get());
        }
    }

    if (auto model = m_tweaker->TextureAttributeModel())
    {
        for (int i = 0; i < Tweakable::Texture::Max; ++i)
        {
            if (m_cache->Texture[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Texture[i].GetUpdated());
            }
        }
    }

    if (m_cache->TexturePath.RequiresUpdate())
    {
        m_tweaker->SetTexturePath(QString::fromStdString(m_cache->TexturePath.GetUpdated()));
    }
}

void QtGui::UpdateLight()
{
    if (auto model = m_tweaker->LightsModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Lights.Get());
            model->SetSelectedIndex(m_cache->LightSelected.Get());
        }
    }

    if (auto model = m_tweaker->LightAttributeModel())
    {
        for (int i = 0; i < Tweakable::Light::Max; ++i)
        {
            if (m_cache->Light[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Light[i].GetUpdated());
            }
        }
    }
}

void QtGui::UpdateMesh()
{
    if (auto model = m_tweaker->MeshesModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Meshes.Get());
            model->SetSelectedIndex(m_cache->MeshSelected.Get());
        }
    }

    if (auto model = m_tweaker->MeshAttributeModel())
    {
        for (int i = 0; i < Tweakable::Mesh::Max; ++i)
        {
            if (m_cache->Mesh[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Mesh[i].GetUpdated());
            }
        }
    }

    if (m_cache->MeshShader.RequiresUpdate())
    {
        m_tweaker->SetMeshShader(QString::fromStdString(m_cache->MeshShader.GetUpdated()));
    }

    if (m_cache->MeshInstances.RequiresUpdate())
    {
        m_tweaker->SetMeshInstances(QString::fromStdString(m_cache->MeshInstances.GetUpdated()));
    }
}

void QtGui::UpdateEmitter()
{
    if (auto model = m_tweaker->EmittersModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Emitters.Get());
            model->SetSelectedIndex(m_cache->EmitterSelected.Get());
        }
    }

    if (auto model = m_tweaker->EmitterAttributeModel())
    {
        for (int i = 0; i < Tweakable::Emitter::Max; ++i)
        {
            if (m_cache->Emitter[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Emitter[i].GetUpdated());
            }
        }
    }

    if (m_cache->EmitterInstances.RequiresUpdate())
    {
        m_tweaker->SetEmitterInstances(QString::fromStdString(m_cache->EmitterInstances.GetUpdated()));
    }
}

void QtGui::UpdateWater()
{
    if (auto model = m_tweaker->WaterModel())
    {
        if (model->Empty())
        {
            model->SetStringList(m_cache->Waters.Get());
            model->SetSelectedIndex(m_cache->WaterSelected.Get());
        }
    }

    if (auto model = m_tweaker->WaterAttributeModel())
    {
        for (int i = 0; i < Tweakable::Water::Max; ++i)
        {
            if (m_cache->Water[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Water[i].GetUpdated());
            }
        }
    }

    if (auto model = m_tweaker->WaveAttributeModel())
    {
        for (int i = 0; i < Tweakable::Wave::Max; ++i)
        {
            if (m_cache->Wave[i].RequiresUpdate())
            {
                model->SetAttributeValue(i, m_cache->Wave[i].GetUpdated());
            }
        }
    }

    if (m_cache->WaveAmount.RequiresUpdate())
    {
        m_tweaker->SetWaveCount(m_cache->WaveAmount.GetUpdated());
    }

    if (m_cache->WaterInstances.RequiresUpdate())
    {
        m_tweaker->SetWaterInstances(QString::fromStdString(m_cache->WaterInstances.GetUpdated()));
    }
}

void QtGui::SetupConnections()
{
    connect(m_editor.get(), &EditorModel::RequestCompileSelectedShader, this,
        [this](const QString& text) { m_cache->CompileShader.Set(text.toStdString()); });
    connect(m_editor->ShadersModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->ShaderSelected.Set(index); });

    connect(m_tweaker->MeshesModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->MeshSelected.Set(index); });
    connect(m_tweaker->WavesModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->WaveSelected.Set(index); });
    connect(m_tweaker->EnginesModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->EngineSelected.Set(index); });
    connect(m_tweaker->WaterModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->WaterSelected.Set(index); });
    connect(m_tweaker->LightsModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->LightSelected.Set(index); });
    connect(m_tweaker->TexturesModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->TextureSelected.Set(index); });
    connect(m_tweaker->EmittersModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->EmitterSelected.Set(index); });
    connect(m_tweaker->TerrainModel(), &StringListModel::SelectedIndexChanged, this,
        [this](int index) { m_cache->TerrainSelected.Set(index); });
    connect(m_tweaker->PostMapsModel(), &StringListModel::SelectedIndexChanged, this,
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