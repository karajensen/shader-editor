////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "cache.h"

PostProcessing::PostProcessing(const boost::property_tree::ptree& node)
{
    SetPostMap(SCENE_MAP);

    m_blurStep = GetValue<float>(node, "BlurStep");
    m_depthFar = GetValue<float>(node, "DepthFar");
    m_depthNear = GetValue<float>(node, "DepthNear");
    m_dofStart = GetValue<float>(node, "DOFStart");
    m_dofFade = GetValue<float>(node, "DOFFade");
    m_fogColour.r = GetAttribute<float>(node, "FogColour", "r");
    m_fogColour.g = GetAttribute<float>(node, "FogColour", "g");
    m_fogColour.b = GetAttribute<float>(node, "FogColour", "b");
    m_fogStart = GetValue<float>(node, "FogStart");
    m_fogFade = GetValue<float>(node, "FogFade");
    m_bloomIntensity = GetValue<float>(node, "BloomIntensity");
    m_bloomStart = GetValue<float>(node, "BloomStart");
    m_bloomFade = GetValue<float>(node, "BloomFade");
    m_contrast = GetValue<float>(node, "Contrast");
    m_saturation = GetValue<float>(node, "Saturation");
    m_maximumColour.r = GetAttribute<float>(node, "MaximumColour", "r");
    m_maximumColour.g = GetAttribute<float>(node, "MaximumColour", "g");
    m_maximumColour.b = GetAttribute<float>(node, "MaximumColour", "b");
    m_minimumColour.r = GetAttribute<float>(node, "MinimumColour", "r");
    m_minimumColour.g = GetAttribute<float>(node, "MinimumColour", "g");
    m_minimumColour.b = GetAttribute<float>(node, "MinimumColour", "b");

    m_weights[0] = 1.0f;
    m_weights[1] = 0.9f;
    m_weights[2] = 0.55f;
    m_weights[3] = 0.18f;
    m_weights[4] = 0.1f;
    NormaliseWeights();
}

void PostProcessing::Write(boost::property_tree::ptree& node) const
{
    node.add("BlurStep", m_blurStep);
    node.add("DepthFar", m_depthFar);
    node.add("DepthNear", m_depthNear);
    node.add("DOFStart", m_dofStart);
    node.add("DOFFade", m_dofFade);
    node.add("FogColour.<xmlattr>.r", m_fogColour.r);
    node.add("FogColour.<xmlattr>.g", m_fogColour.g);
    node.add("FogColour.<xmlattr>.b", m_fogColour.b);
    node.add("FogStart", m_fogStart);
    node.add("FogFade", m_fogFade);
    node.add("BloomIntensity", m_bloomIntensity);
    node.add("BloomStart", m_bloomStart);
    node.add("BloomFade", m_bloomFade);
    node.add("Contrast", m_contrast);
    node.add("Saturation", m_saturation);
    node.add("MaximumColour.<xmlattr>.r", m_maximumColour.r);
    node.add("MaximumColour.<xmlattr>.g", m_maximumColour.g);
    node.add("MaximumColour.<xmlattr>.b", m_maximumColour.b);
    node.add("MinimumColour.<xmlattr>.r", m_minimumColour.r);
    node.add("MinimumColour.<xmlattr>.g", m_minimumColour.g);
    node.add("MinimumColour.<xmlattr>.b", m_minimumColour.b);
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
    case NORMAL_MAP:
        return "Normal Map";
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