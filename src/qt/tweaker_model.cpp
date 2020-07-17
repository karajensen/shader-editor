////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker_model.h"
#include "attribute_model.h"
#include "attribute_filter_model.h"
#include "stringlist_model.h"
#include "logger.h"

#include <QFileInfo>
#include <QLocale>

namespace
{
    // Ensure the Qml exposed enums match with the application enums
    static_assert((int)GuiPage::Scene == (int)TweakerModel::TabPage::Scene, "Enums must match");
    static_assert((int)GuiPage::Area  == (int)TweakerModel::TabPage::Area,  "Enums must match");
    static_assert((int)GuiPage::Post  == (int)TweakerModel::TabPage::Post,  "Enums must match");
    static_assert((int)GuiPage::Mesh  == (int)TweakerModel::TabPage::Mesh,  "Enums must match");

    template<typename T>
    AttributeModel::AttributeData createAttribute(typename T::Attribute attribute, 
                                                  int stepPrecision = 3, 
                                                  bool enabled = true)
    {
        return { attribute, T::group(attribute), T::toString(attribute), stepPrecision, enabled };
    }
}

TweakerModel::TweakerModel(QObject* parent)
    : QObject(parent)
    , m_selectedPage(TabPage::Scene)
    , m_meshAttributeModel(new AttributeModel(this))
    , m_waveAttributeModel(new AttributeModel(this))
    , m_waterAttributeModel(new AttributeModel(this))
    , m_lightAttributeModel(new AttributeModel(this))
    , m_textureAttributeModel(new AttributeModel(this))
    , m_emitterAttributeModel(new AttributeModel(this))
    , m_terrainAttributeModel(new AttributeModel(this))
    , m_postAttributeModel(new AttributeModel(this))
    , m_cameraAttributeModel(new AttributeModel(this))
    , m_enginesModel(new StringListModel(this))
    , m_meshesModel(new StringListModel(this))
    , m_wavesModel(new StringListModel(this))
    , m_waterModel(new StringListModel(this))
    , m_lightsModel(new StringListModel(this))
    , m_texturesModel(new StringListModel(this))
    , m_emittersModel(new StringListModel(this))
    , m_terrainModel(new StringListModel(this))
    , m_postMapsModel(new StringListModel(this))
    , m_lightAttributeFilterModel(new AttributeFilterModel(this))
    , m_lightAttenuationAttributeFilterModel(new AttributeFilterModel(this))
    , m_lightPositionAttributeFilterModel(new AttributeFilterModel(this))
    , m_lightSpecularAttributeFilterModel(new AttributeFilterModel(this))
    , m_lightDiffuseAttributeFilterModel(new AttributeFilterModel(this))
    , m_emitterAttributeFilterModel(new AttributeFilterModel(this))
    , m_emitterMinMaxAttributeFilterModel(new AttributeFilterModel(this))
    , m_postAttributeFilterModel(new AttributeFilterModel(this))
    , m_postCorrectionAttributeFilterModel(new AttributeFilterModel(this))
    , m_postFogAttributeFilterModel(new AttributeFilterModel(this))
{
    m_cameraAttributeModel->SetAttributes(
    {
        createAttribute<Camera>(Camera::PositionX, 3, false),
        createAttribute<Camera>(Camera::PositionY, 3, false),
        createAttribute<Camera>(Camera::PositionZ, 3, false),
        createAttribute<Camera>(Camera::Pitch, 3, false),
        createAttribute<Camera>(Camera::Yaw, 3, false),
        createAttribute<Camera>(Camera::Roll, 3, false),
        createAttribute<Camera>(Camera::ForwardSpd),
        createAttribute<Camera>(Camera::RotationSpd)
    });
    if (m_cameraAttributeModel->rowCount() != Camera::Max)
    {
        Logger::LogError("Camera attributes don't match enum");
    }

    m_textureAttributeModel->SetAttributes(
    {
        createAttribute<Texture>(Texture::Contrast),
        createAttribute<Texture>(Texture::Amplitude),
        createAttribute<Texture>(Texture::Scale),
        createAttribute<Texture>(Texture::Iterations, 0)
    });
    if (m_textureAttributeModel->rowCount() != Texture::Max)
    {
        Logger::LogError("Texture attributes don't match enum");
    }

    m_terrainAttributeModel->SetAttributes(
    {
        createAttribute<Terrain>(Terrain::Bump),
        createAttribute<Terrain>(Terrain::Ambience),
        createAttribute<Terrain>(Terrain::CausticsAmount),
        createAttribute<Terrain>(Terrain::CausticsScale),
        createAttribute<Terrain>(Terrain::Specularity),
        createAttribute<Terrain>(Terrain::MinHeight),
        createAttribute<Terrain>(Terrain::MaxHeight),
        createAttribute<Terrain>(Terrain::Scale, 0)
    });
    if (m_terrainAttributeModel->rowCount() != Terrain::Max)
    {
        Logger::LogError("Terrain attributes don't match enum");
    }

    m_postAttributeModel->SetAttributes(
    {
        createAttribute<Post>(Post::DepthNear, 1),
        createAttribute<Post>(Post::DepthFar, 0),
        createAttribute<Post>(Post::BlurStep, 3),
        createAttribute<Post>(Post::CausticSpeed, 3),
        createAttribute<Post>(Post::BloomAmount, 3),
        createAttribute<Post>(Post::BloomStart, 3),
        createAttribute<Post>(Post::BloomFade, 3),
        createAttribute<Post>(Post::DofStart),
        createAttribute<Post>(Post::DofFade),
        createAttribute<Post>(Post::Contrast),
        createAttribute<Post>(Post::Saturation),
        createAttribute<Post>(Post::MinRed),
        createAttribute<Post>(Post::MinGreen),
        createAttribute<Post>(Post::MinBlue),
        createAttribute<Post>(Post::MaxRed),
        createAttribute<Post>(Post::MaxGreen),
        createAttribute<Post>(Post::MaxBlue),
        createAttribute<Post>(Post::FogStart),
        createAttribute<Post>(Post::FogFade),
        createAttribute<Post>(Post::FogRed),
        createAttribute<Post>(Post::FogGreen),
        createAttribute<Post>(Post::FogBlue)
    });
    if (m_postAttributeModel->rowCount() != Post::Max)
    {
        Logger::LogError("Post attributes don't match enum");
    }

    m_lightAttributeModel->SetAttributes(
    {
        createAttribute<Light>(Light::Active, 1),
        createAttribute<Light>(Light::AttenuationX, 3),
        createAttribute<Light>(Light::AttenuationY, 3),
        createAttribute<Light>(Light::AttenuationZ, 3),
        createAttribute<Light>(Light::PositionX, 1),
        createAttribute<Light>(Light::PositionY, 1),
        createAttribute<Light>(Light::PositionZ, 1),
        createAttribute<Light>(Light::DiffuseR),
        createAttribute<Light>(Light::DiffuseG),
        createAttribute<Light>(Light::DiffuseB),
        createAttribute<Light>(Light::SpecularR),
        createAttribute<Light>(Light::SpecularG),
        createAttribute<Light>(Light::SpecularB),
        createAttribute<Light>(Light::Specularity)
    });
    if (m_lightAttributeModel->rowCount() != Light::Max)
    {
        Logger::LogError("Light attributes don't match enum");
    }

    m_meshAttributeModel->SetAttributes(
    {
        createAttribute<Mesh>(Mesh::Ambience),
        createAttribute<Mesh>(Mesh::CausticsAmount),
        createAttribute<Mesh>(Mesh::CausticsScale),
        createAttribute<Mesh>(Mesh::Bump),
        createAttribute<Mesh>(Mesh::Specularity)
    });
    if (m_meshAttributeModel->rowCount() != Mesh::Max)
    {
        Logger::LogError("Mesh attributes don't match enum");
    }

    m_emitterAttributeModel->SetAttributes(
    {
        createAttribute<Emitter>(Emitter::LifeTime),
        createAttribute<Emitter>(Emitter::LifeFade),
        createAttribute<Emitter>(Emitter::Length),
        createAttribute<Emitter>(Emitter::Width),
        createAttribute<Emitter>(Emitter::DirX),
        createAttribute<Emitter>(Emitter::DirY),
        createAttribute<Emitter>(Emitter::DirZ),
        createAttribute<Emitter>(Emitter::TintR),
        createAttribute<Emitter>(Emitter::TintG),
        createAttribute<Emitter>(Emitter::TintB),
        createAttribute<Emitter>(Emitter::MaxSpeed),
        createAttribute<Emitter>(Emitter::MinSpeed),
        createAttribute<Emitter>(Emitter::MaxSize),
        createAttribute<Emitter>(Emitter::MinSize),
        createAttribute<Emitter>(Emitter::MaxAmp),
        createAttribute<Emitter>(Emitter::MinAmp),
        createAttribute<Emitter>(Emitter::MaxFreq),
        createAttribute<Emitter>(Emitter::MinFreq),
        createAttribute<Emitter>(Emitter::MaxWait),
        createAttribute<Emitter>(Emitter::MinWait)
    });
    if (m_emitterAttributeModel->rowCount() != Emitter::Max)
    {
        Logger::LogError("Emitter attributes don't match enum");
    }

    m_waterAttributeModel->SetAttributes(
    {
        createAttribute<Water>(Water::Speed),
        createAttribute<Water>(Water::ShallowA),
        createAttribute<Water>(Water::ShallowR),
        createAttribute<Water>(Water::ShallowG),
        createAttribute<Water>(Water::ShallowB),
        createAttribute<Water>(Water::DeepA),
        createAttribute<Water>(Water::DeepR),
        createAttribute<Water>(Water::DeepG),
        createAttribute<Water>(Water::DeepB),
        createAttribute<Water>(Water::Reflection),
        createAttribute<Water>(Water::ReflectionR),
        createAttribute<Water>(Water::ReflectionG),
        createAttribute<Water>(Water::ReflectionB),
        createAttribute<Water>(Water::Bump),
        createAttribute<Water>(Water::BumpScaleX),
        createAttribute<Water>(Water::BumpScaleY),
        createAttribute<Water>(Water::ScaleU),
        createAttribute<Water>(Water::ScaleV),
        createAttribute<Water>(Water::FresnalBias),
        createAttribute<Water>(Water::FresnalPower),
        createAttribute<Water>(Water::FresnalScale),
    });
    if (m_waterAttributeModel->rowCount() != Water::Max)
    {
        Logger::LogError("Water attributes don't match enum");
    }

    m_waveAttributeModel->SetAttributes(
    {
        createAttribute<Wave>(Wave::Amplitude),
        createAttribute<Wave>(Wave::Frequency),
        createAttribute<Wave>(Wave::Phase),
        createAttribute<Wave>(Wave::DirectionX),
        createAttribute<Wave>(Wave::DirectionZ)
    });
    if (m_waveAttributeModel->rowCount() != Wave::Max)
    {
        Logger::LogError("Wave attributes don't match enum");
    }

    m_lightAttributeFilterModel->setSourceModel(m_lightAttributeModel);
    m_lightAttributeFilterModel->setFilterFixedString(QString::number(Light::Group::None));

    m_lightAttenuationAttributeFilterModel->setSourceModel(m_lightAttributeModel);
    m_lightAttenuationAttributeFilterModel->setFilterFixedString(QString::number(Light::Group::Attenuation));
    
    m_lightPositionAttributeFilterModel->setSourceModel(m_lightAttributeModel);
    m_lightPositionAttributeFilterModel->setFilterFixedString(QString::number(Light::Group::Position));

    m_lightSpecularAttributeFilterModel->setSourceModel(m_lightAttributeModel);
    m_lightSpecularAttributeFilterModel->setFilterFixedString(QString::number(Light::Group::Specular));

    m_lightDiffuseAttributeFilterModel->setSourceModel(m_lightAttributeModel);
    m_lightDiffuseAttributeFilterModel->setFilterFixedString(QString::number(Light::Group::Diffuse));

    m_emitterAttributeFilterModel->setSourceModel(m_emitterAttributeModel);
    m_emitterAttributeFilterModel->setFilterFixedString(QString::number(Emitter::Group::None));

    m_emitterMinMaxAttributeFilterModel->setSourceModel(m_emitterAttributeModel);
    m_emitterMinMaxAttributeFilterModel->setFilterFixedString(QString::number(Emitter::Group::MinMax));

    m_postAttributeFilterModel->setSourceModel(m_postAttributeModel);
    m_postAttributeFilterModel->setFilterFixedString(QString::number(Post::Group::None));

    m_postCorrectionAttributeFilterModel->setSourceModel(m_postAttributeModel);
    m_postCorrectionAttributeFilterModel->setFilterFixedString(QString::number(Post::Group::Correction));

    m_postFogAttributeFilterModel->setSourceModel(m_postAttributeModel);
    m_postFogAttributeFilterModel->setFilterFixedString(QString::number(Post::Group::Fog));
}

void TweakerModel::SetSelectedPage(TabPage page)
{
    if (m_selectedPage != page)
    {
        m_selectedPage = page;
        emit SelectedPageChanged();
    }
}

TweakerModel::TabPage TweakerModel::SelectedPage() const
{
    return m_selectedPage;
}

AttributeModel* TweakerModel::MeshAttributeModel() const
{
    return m_meshAttributeModel;
}

AttributeModel* TweakerModel::WaveAttributeModel() const
{
    return m_waveAttributeModel;
}

AttributeModel* TweakerModel::WaterAttributeModel() const
{
    return m_waterAttributeModel;
}

AttributeModel* TweakerModel::LightAttributeModel() const
{
    return m_lightAttributeModel;
}

AttributeModel* TweakerModel::TextureAttributeModel() const
{
    return m_textureAttributeModel;
}

AttributeModel* TweakerModel::EmitterAttributeModel() const
{
    return m_emitterAttributeModel;
}

AttributeModel* TweakerModel::TerrainAttributeModel() const
{
    return m_terrainAttributeModel;
}

AttributeModel* TweakerModel::PostAttributeModel() const
{
    return m_postAttributeModel;
}

AttributeModel* TweakerModel::CameraAttributeModel() const
{
    return m_cameraAttributeModel;
}

AttributeFilterModel* TweakerModel::LightAttributeFilterModel() const
{
    return m_lightAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::LightAttenuationAttributeFilterModel() const
{
    return m_lightAttenuationAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::LightPositionAttributeFilterModel() const
{
    return m_lightPositionAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::LightSpecularAttributeFilterModel() const
{
    return m_lightSpecularAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::LightDiffuseAttributeFilterModel() const
{
    return m_lightDiffuseAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::EmitterAttributeFilterModel() const
{
    return m_emitterAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::EmitterMinMaxAttributeFilterModel() const
{
    return m_emitterMinMaxAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::PostAttributeFilterModel() const
{
    return m_postAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::PostCorrectionAttributeFilterModel() const
{
    return m_postCorrectionAttributeFilterModel;
}

AttributeFilterModel* TweakerModel::PostFogAttributeFilterModel() const
{
    return m_postFogAttributeFilterModel;
}

StringListModel* TweakerModel::EnginesModel() const
{
    return m_enginesModel;
}

StringListModel* TweakerModel::PostMapsModel() const
{
    return m_postMapsModel;
}

StringListModel* TweakerModel::MeshesModel() const
{
    return m_meshesModel;
}

StringListModel* TweakerModel::TerrainModel() const
{
    return m_terrainModel;
}

StringListModel* TweakerModel::TexturesModel() const
{
    return m_texturesModel;
}

StringListModel* TweakerModel::EmittersModel() const
{
    return m_emittersModel;
}

StringListModel* TweakerModel::WaterModel() const
{
    return m_waterModel;
}

StringListModel* TweakerModel::LightsModel() const
{
    return m_lightsModel;
}

StringListModel* TweakerModel::WavesModel() const
{
    return m_wavesModel;
}

void TweakerModel::SetWaveCount(int count)
{
    if (m_waveCount != count)
    {
        m_waveCount = count;
        emit WaveCountChanged();
    }
}

int TweakerModel::WaveCount() const
{
    return m_waveCount;
}

void TweakerModel::SetDeltaTime(float deltaTime)
{
    m_deltaTime = deltaTime;
    emit DeltaTimeChanged();
}

QString TweakerModel::DeltaTime() const
{
    return QLocale().toString(m_deltaTime, 'f', 8);
}

void TweakerModel::SetFramesPerSecond(int fps)
{
    if (m_framesPerSecond != fps)
    {
        m_framesPerSecond = fps;
        emit FramesPerSecondChanged();
    }
}

int TweakerModel::FramesPerSecond() const
{
    return m_framesPerSecond;
}

void TweakerModel::SetMeshShader(const QString& shader)
{
    if (m_meshShader != shader)
    {
        m_meshShader = shader;
        emit MeshShaderChanged();
    }
}

const QString& TweakerModel::MeshShader() const
{
    return m_meshShader;
}

void TweakerModel::SetTerrainShader(const QString& shader)
{
    if (m_terrainShader != shader)
    {
        m_terrainShader = shader;
        emit TerrainShaderChanged();
    }
}

const QString& TweakerModel::TerrainShader() const
{
    return m_terrainShader;
}

void TweakerModel::SetTexturePath(const QString& path)
{
    const auto filePath = "file:///" + QFileInfo(path).absoluteFilePath();
    if (m_texturePath != filePath)
    {
        m_texturePath = filePath;
        emit TexturePathChanged();
    }
}

const QString& TweakerModel::TexturePath() const
{
    return m_texturePath;
}

void TweakerModel::SetMeshInstances(const QString& instances)
{
    if (m_meshInstances != instances)
    {
        m_meshInstances = instances;
        emit MeshInstancesChanged();
    }
}

const QString& TweakerModel::MeshInstances() const
{
    return m_meshInstances;
}

void TweakerModel::SetEmitterInstances(const QString& instances)
{
    if (m_emitterInstances != instances)
    {
        m_emitterInstances = instances;
        emit EmitterInstancesChanged();
    }
}

const QString& TweakerModel::EmitterInstances() const
{
    return m_emitterInstances;
}

void TweakerModel::SetTerrainInstances(const QString& instances)
{
    if (m_terrainInstances != instances)
    {
        m_terrainInstances = instances;
        emit TerrainInstancesChanged();
    }
}

const QString& TweakerModel::TerrainInstances() const
{
    return m_terrainInstances;
}

void TweakerModel::SetWaterInstances(const QString& instances)
{
    if (m_waterInstances != instances)
    {
        m_waterInstances = instances;
        emit WaterInstancesChanged();
    }
}

const QString& TweakerModel::WaterInstances() const
{
    return m_waterInstances;
}

void TweakerModel::ReloadScene()
{
    emit RequestReloadScene();
}

void TweakerModel::ReloadEngine()
{
    emit RequestReloadEngine();
}

void TweakerModel::ReloadTerrain()
{
    emit RequestReloadTerrain();
}

void TweakerModel::ReloadTexture()
{
    emit RequestReloadTexture();
}

void TweakerModel::ReloadPlacement()
{
    emit RequestReloadPlacement();
}

void TweakerModel::ToggleWireframe()
{
    emit RequestToggleWireframe();
}

void TweakerModel::TogglePauseEmission()
{
    emit RequestTogglePauseEmission();
}

void TweakerModel::ToggleLightsOnly()
{
    emit RequestToggleLightsOnly();
}

void TweakerModel::ToggleLightsDiagnostics()
{
    emit RequestToggleLightsDiagnostics();
}

QString TweakerModel::tabPageName(TabPage page) const
{
    switch (page)
    {
    case TabPage::Scene:
        return tr("Scene");
    case TabPage::Area:
        return tr("Area");
    case TabPage::Mesh:
        return tr("Mesh");
    case TabPage::Post:
        return tr("Post");
    }
    Q_UNREACHABLE();
    return QString();
}
