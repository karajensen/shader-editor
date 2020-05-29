////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "cache.h"

PostProcessing::PostProcessing()
{
    SetPostMap(Map::Final);

    m_blurStep = 0.003f;
    m_depthFar = 1000.0f;
    m_depthNear = 1.0f;
    m_dofStart = 0.99f;
    m_dofFade = 0.01f;
    m_fogColour.r = 0.0f;
    m_fogColour.g = 100.0f/255.0f;
    m_fogColour.b = 1.0f;
    m_fogStart = 0.39f;
    m_fogFade = 0.61f;
    m_bloomIntensity = 0.65f;
    m_bloomStart = 0.95f;
    m_bloomFade = 0.35f;
    m_contrast = 0.3f;
    m_saturation = 1.0f;
    m_maximumColour.r = 1.0f;
    m_maximumColour.g = 1.0f;
    m_maximumColour.b = 220.0f/255.0f;
    m_minimumColour.r = 0.0f;
    m_minimumColour.g = 0.0f;
    m_minimumColour.b = 0.0f;

    const int BLUR_PIXELS = 5;
    m_weights.resize(BLUR_PIXELS);

    m_weights[0] = 1.0f;
    m_weights[1] = 0.9f;
    m_weights[2] = 0.55f;
    m_weights[3] = 0.18f;
    m_weights[4] = 0.1f;

    // Normalise the blurring weights
    const float overallWeight = m_weights[0] + 2.0f * 
        (m_weights[1] + m_weights[2] + m_weights[3] + m_weights[4]);

    m_weights[0] /= overallWeight;
    m_weights[1] /= overallWeight;
    m_weights[2] /= overallWeight;
    m_weights[3] /= overallWeight;
    m_weights[4] /= overallWeight;
}

void PostProcessing::Write(Cache& cache)
{
    cache.Post[Tweakable::Post::DepthNear].SetUpdated(m_depthNear);
    cache.Post[Tweakable::Post::DepthFar].SetUpdated(m_depthFar);
    cache.Post[Tweakable::Post::DofStart].SetUpdated(m_dofStart);
    cache.Post[Tweakable::Post::DofFade].SetUpdated(m_dofFade);
    cache.Post[Tweakable::Post::BlurStep].SetUpdated(m_blurStep);
    cache.Post[Tweakable::Post::BloomAmount].SetUpdated(m_bloomIntensity);
    cache.Post[Tweakable::Post::BloomStart].SetUpdated(m_bloomStart);
    cache.Post[Tweakable::Post::BloomFade].SetUpdated(m_bloomFade);
    cache.Post[Tweakable::Post::Contrast].SetUpdated(m_contrast);
    cache.Post[Tweakable::Post::Saturation].SetUpdated(m_saturation);
    cache.Post[Tweakable::Post::FogRed].SetUpdated(m_fogColour.r);
    cache.Post[Tweakable::Post::FogGreen].SetUpdated(m_fogColour.g);
    cache.Post[Tweakable::Post::FogBlue].SetUpdated(m_fogColour.b);
    cache.Post[Tweakable::Post::FogStart].SetUpdated(m_fogStart);
    cache.Post[Tweakable::Post::FogFade].SetUpdated(m_fogFade);
    cache.Post[Tweakable::Post::MinRed].SetUpdated(m_minimumColour.r);
    cache.Post[Tweakable::Post::MinGreen].SetUpdated(m_minimumColour.g);
    cache.Post[Tweakable::Post::MinBlue].SetUpdated(m_minimumColour.b);
    cache.Post[Tweakable::Post::MaxRed].SetUpdated(m_maximumColour.r);
    cache.Post[Tweakable::Post::MaxGreen].SetUpdated(m_maximumColour.g);
    cache.Post[Tweakable::Post::MaxBlue].SetUpdated(m_maximumColour.b);
}

void PostProcessing::Read(Cache& cache)
{
    m_dofStart = cache.Post[Tweakable::Post::DofStart].Get();
    m_dofFade = cache.Post[Tweakable::Post::DofFade].Get();
    m_blurStep = cache.Post[Tweakable::Post::BlurStep].Get();
    m_bloomIntensity = cache.Post[Tweakable::Post::BloomAmount].Get();
    m_bloomStart = cache.Post[Tweakable::Post::BloomStart].Get();
    m_bloomFade = cache.Post[Tweakable::Post::BloomFade].Get();
    m_contrast = cache.Post[Tweakable::Post::Contrast].Get();
    m_saturation = cache.Post[Tweakable::Post::Saturation].Get();
    m_depthFar = cache.Post[Tweakable::Post::DepthFar].Get();
    m_depthNear = cache.Post[Tweakable::Post::DepthNear].Get();
    m_fogColour.r = cache.Post[Tweakable::Post::FogRed].Get();
    m_fogColour.g = cache.Post[Tweakable::Post::FogGreen].Get();
    m_fogColour.b = cache.Post[Tweakable::Post::FogBlue].Get();
    m_fogStart = cache.Post[Tweakable::Post::FogStart].Get();
    m_fogFade = cache.Post[Tweakable::Post::FogFade].Get();
    m_minimumColour.r = cache.Post[Tweakable::Post::MinRed].Get();
    m_minimumColour.g = cache.Post[Tweakable::Post::MinGreen].Get();
    m_minimumColour.b = cache.Post[Tweakable::Post::MinBlue].Get();
    m_maximumColour.r = cache.Post[Tweakable::Post::MaxRed].Get();
    m_maximumColour.g = cache.Post[Tweakable::Post::MaxGreen].Get();
    m_maximumColour.b = cache.Post[Tweakable::Post::MaxBlue].Get();
}

void PostProcessing::NormaliseWeights()
{
    const float overallWeight = m_weights[0] + 2.0f * 
        (m_weights[1] + m_weights[2] + m_weights[3] + m_weights[4]);

    m_weights[0] /= overallWeight;
    m_weights[1] /= overallWeight;
    m_weights[2] /= overallWeight;
    m_weights[3] /= overallWeight;
    m_weights[4] /= overallWeight;
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
    m_masks.assign(0.0f);
    m_masks[map] = 1.0f;
}

std::string PostProcessing::GetMapName(PostProcessing::Map map)
{
    switch (map)
    {
    case Map::Final:
        return "Final Scene";
    case Map::Scene:
        return "Scene Map";
    case Map::Depth:
        return "Depth Map";
    case Map::Blur:
        return "Blur Map";
    case Map::Dof:
        return "DOF Map";
    case Map::Fog:
        return "Fog Map";
    case Map::Bloom:
        return "Bloom Map";
    default:
        return "None";
    }
}

const float& PostProcessing::DOFStart() const
{
    return m_dofStart;
}

const float& PostProcessing::DOFFade() const
{
    return m_dofFade;
}

const float& PostProcessing::BloomIntensity() const
{
    return m_bloomIntensity;
}

const float& PostProcessing::BloomStart() const
{
    return m_bloomStart;
}

const float& PostProcessing::BloomFade() const
{
    return m_bloomFade;
}

const std::vector<float>& PostProcessing::GetBlurWeights() const
{
    return m_weights;
}

const float& PostProcessing::Contrast() const
{
    return m_contrast;
}

const float& PostProcessing::Saturation() const
{
    return m_saturation;
}

const float& PostProcessing::BlurStep() const
{
    return m_blurStep;
}

const float& PostProcessing::DepthNear() const
{
    return m_depthNear;
}

const float& PostProcessing::DepthFar() const
{
    return m_depthFar;
}

const float& PostProcessing::FogStart() const
{
    return m_fogStart;
}

const float& PostProcessing::FogFade() const
{
    return m_fogFade;
}

const Colour& PostProcessing::FogColour() const
{
    return m_fogColour;
}

const Colour& PostProcessing::MinColour() const
{
    return m_minimumColour;
}

const Colour& PostProcessing::MaxColour() const
{
    return m_maximumColour;
}

const float& PostProcessing::Mask(PostProcessing::Map map) const
{
    return m_masks.at(map);
}

void PostProcessing::ToggleDiffuseTextures()
{
    m_useDiffuseTextures = !m_useDiffuseTextures;
}

bool PostProcessing::UseDiffuseTextures() const
{
    return m_useDiffuseTextures;
}