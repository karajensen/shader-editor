////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "cache.h"

PostProcessing::PostProcessing()
{
    SetPostMap(FINAL_MAP);

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
    cache.Post[POST_DEPTH_NEAR].SetUpdated(m_depthNear);
    cache.Post[POST_DEPTH_FAR].SetUpdated(m_depthFar);
    cache.Post[POST_DOF_START].SetUpdated(m_dofStart);
    cache.Post[POST_DOF_FADE].SetUpdated(m_dofFade);
    cache.Post[POST_BLUR_STEP].SetUpdated(m_blurStep);
    cache.Post[POST_BLOOM_AMOUNT].SetUpdated(m_bloomIntensity);
    cache.Post[POST_BLOOM_START].SetUpdated(m_bloomStart);
    cache.Post[POST_BLOOM_FADE].SetUpdated(m_bloomFade);
    cache.Post[POST_CONTRAST].SetUpdated(m_contrast);
    cache.Post[POST_SATURATION].SetUpdated(m_saturation);
    cache.Post[POST_FOG_RED].SetUpdated(m_fogColour.r);
    cache.Post[POST_FOG_GREEN].SetUpdated(m_fogColour.g);
    cache.Post[POST_FOG_BLUE].SetUpdated(m_fogColour.b);
    cache.Post[POST_FOG_START].SetUpdated(m_fogStart);
    cache.Post[POST_FOG_FADE].SetUpdated(m_fogFade);
    cache.Post[POST_MIN_RED].SetUpdated(m_minimumColour.r);
    cache.Post[POST_MIN_GREEN].SetUpdated(m_minimumColour.g);
    cache.Post[POST_MIN_BLUE].SetUpdated(m_minimumColour.b);
    cache.Post[POST_MAX_RED].SetUpdated(m_maximumColour.r);
    cache.Post[POST_MAX_GREEN].SetUpdated(m_maximumColour.g);
    cache.Post[POST_MAX_BLUE].SetUpdated(m_maximumColour.b);
}

void PostProcessing::Read(Cache& cache)
{
    m_dofStart = cache.Post[POST_DOF_START].Get();
    m_dofFade = cache.Post[POST_DOF_FADE].Get();
    m_blurStep = cache.Post[POST_BLUR_STEP].Get();
    m_bloomIntensity = cache.Post[POST_BLOOM_AMOUNT].Get();
    m_bloomStart = cache.Post[POST_BLOOM_START].Get();
    m_bloomFade = cache.Post[POST_BLOOM_FADE].Get();
    m_contrast = cache.Post[POST_CONTRAST].Get();
    m_saturation = cache.Post[POST_SATURATION].Get();
    m_depthFar = cache.Post[POST_DEPTH_FAR].Get();
    m_depthNear = cache.Post[POST_DEPTH_NEAR].Get();
    m_fogColour.r = cache.Post[POST_FOG_RED].Get();
    m_fogColour.g = cache.Post[POST_FOG_GREEN].Get();
    m_fogColour.b = cache.Post[POST_FOG_BLUE].Get();
    m_fogStart = cache.Post[POST_FOG_START].Get();
    m_fogFade = cache.Post[POST_FOG_FADE].Get();
    m_minimumColour.r = cache.Post[POST_MIN_RED].Get();
    m_minimumColour.g = cache.Post[POST_MIN_GREEN].Get();
    m_minimumColour.b = cache.Post[POST_MIN_BLUE].Get();
    m_maximumColour.r = cache.Post[POST_MAX_RED].Get();
    m_maximumColour.g = cache.Post[POST_MAX_GREEN].Get();
    m_maximumColour.b = cache.Post[POST_MAX_BLUE].Get();
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
    case FINAL_MAP:
        return "Final Scene";
    case SCENE_MAP:
        return "Scene Map";
    case DEPTH_MAP:
        return "Depth Map";
    case BLUR_MAP:
        return "Blur Map";
    case DOF_MAP:
        return "DOF Map";
    case FOG_MAP:
        return "Fog Map";
    case BLOOM_MAP:
        return "Bloom Map";
    case AMBIENCE_MAP:
        return "Ambience Map";
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

const float& PostProcessing::BlurWeight(int index) const
{
    return m_weights.at(index);
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