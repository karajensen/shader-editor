////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "cache.h"

PostProcessing::PostProcessing(const boost::property_tree::ptree& node)
{
    SetPostMap(SCENE_MAP);

    m_blurAmount = GetValue<float>(node, "BlurAmount");
    m_blurStep = GetValue<float>(node, "BlurStep");
    m_depthFar = GetValue<float>(node, "DepthFar");
    m_depthNear = GetValue<float>(node, "DepthNear");
    m_dofDistance = GetValue<float>(node, "DOFDistance");
    m_dofFade = GetValue<float>(node, "DOFFade");
    m_fogColour.r = GetAttribute<float>(node, "FogColour", "r");
    m_fogColour.g = GetAttribute<float>(node, "FogColour", "g");
    m_fogColour.b = GetAttribute<float>(node, "FogColour", "b");
    m_fogDistance = GetValue<float>(node, "FogDistance");
    m_fogFade = GetValue<float>(node, "FogFade");
    m_glowAmount = GetValue<float>(node, "GlowAmount");
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
    node.add("BlurAmount", m_blurAmount);
    node.add("BlurStep", m_blurStep);
    node.add("DepthFar", m_depthFar);
    node.add("DepthNear", m_depthNear);
    node.add("DOFDistance", m_dofDistance);
    node.add("DOFFade", m_dofFade);
    node.add("FogColour.<xmlattr>.r", m_fogColour.r);
    node.add("FogColour.<xmlattr>.g", m_fogColour.g);
    node.add("FogColour.<xmlattr>.b", m_fogColour.b);
    node.add("FogDistance", m_fogDistance);
    node.add("FogFade", m_fogFade);
    node.add("GlowAmount", m_glowAmount);
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
    cache.DepthNear.SetUpdated(m_depthNear);
    cache.DepthFar.SetUpdated(m_depthFar);
    cache.DOFDistance.SetUpdated(m_dofDistance);
    cache.DOFFade.SetUpdated(m_dofFade);
    cache.BlurAmount.SetUpdated(m_blurAmount);
    cache.BlurStep.SetUpdated(m_blurStep);
    cache.GlowAmount.SetUpdated(m_glowAmount);
    cache.Contrast.SetUpdated(m_contrast);
    cache.Saturation.SetUpdated(m_saturation);
    cache.Fog[FOG_RED].SetUpdated(m_fogColour.r);
    cache.Fog[FOG_GREEN].SetUpdated(m_fogColour.g);
    cache.Fog[FOG_BLUE].SetUpdated(m_fogColour.b);
    cache.Fog[FOG_DISTANCE].SetUpdated(m_fogDistance);
    cache.Fog[FOG_FADE].SetUpdated(m_fogFade);
    cache.MinColour[RED].SetUpdated(m_minimumColour.r);
    cache.MinColour[GREEN].SetUpdated(m_minimumColour.g);
    cache.MinColour[BLUE].SetUpdated(m_minimumColour.b);
    cache.MaxColour[RED].SetUpdated(m_maximumColour.r);
    cache.MaxColour[GREEN].SetUpdated(m_maximumColour.g);
    cache.MaxColour[BLUE].SetUpdated(m_maximumColour.b);
}

void PostProcessing::Read(Cache& cache)
{
    m_dofDistance = cache.DOFDistance.Get();
    m_dofFade = cache.DOFFade.Get();
    m_blurAmount = cache.BlurAmount.Get();
    m_blurStep = cache.BlurStep.Get();
    m_glowAmount = cache.GlowAmount.Get();
    m_contrast = cache.Contrast.Get();
    m_saturation = cache.Saturation.Get();
    m_depthFar = cache.DepthFar.Get();
    m_depthNear = cache.DepthNear.Get();
    m_fogColour.r = cache.Fog[FOG_RED].Get();
    m_fogColour.g = cache.Fog[FOG_GREEN].Get();
    m_fogColour.b = cache.Fog[FOG_BLUE].Get();
    m_fogDistance = cache.Fog[FOG_DISTANCE].Get();
    m_fogFade = cache.Fog[FOG_FADE].Get();
    m_minimumColour.r = cache.MinColour[RED].Get();
    m_minimumColour.g = cache.MinColour[GREEN].Get();
    m_minimumColour.b = cache.MinColour[BLUE].Get();
    m_maximumColour.r = cache.MaxColour[RED].Get();
    m_maximumColour.g = cache.MaxColour[GREEN].Get();
    m_maximumColour.b = cache.MaxColour[BLUE].Get();
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
    case GLOW_MAP:
        return "Glow Map";
    case BLUR_GLOW_MAP:
        return "Blur-Glow Map";
    case BLUR_SCENE_MAP:
        return "Blur-Scene Map";
    case DOF_MAP:
        return "DOF Map";
    case FOG_MAP:
        return "Fog Map";
    default:
        return "None";
    }
}

const float& PostProcessing::DOFDistance() const
{
    return m_dofDistance;
}

const float& PostProcessing::DOFFade() const
{
    return m_dofFade;
}

const float& PostProcessing::Glow() const
{
    return m_glowAmount;
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

const float& PostProcessing::BlurAmount() const
{
    return m_blurAmount;
}

const float& PostProcessing::DepthNear() const
{
    return m_depthNear;
}

const float& PostProcessing::DepthFar() const
{
    return m_depthFar;
}

const float& PostProcessing::FogDistance() const
{
    return m_fogDistance;
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