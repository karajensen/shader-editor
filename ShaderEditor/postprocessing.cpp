////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"

PostProcessing::PostProcessing()
{
    SetPostMap(SCENE_MAP);

    weights[0] = 1.0f;
    weights[1] = 0.9f;
    weights[2] = 0.55f;
    weights[3] = 0.18f;
    weights[4] = 0.1f;
    NormaliseWeights();
}

void PostProcessing::NormaliseWeights()
{
    const float overallWeight = weights[0] + 2.0f * 
        (weights[1] + weights[2] + weights[3] + weights[4]);

    weights[0] /= overallWeight;
    weights[1] /= overallWeight;
    weights[2] /= overallWeight;
    weights[3] /= overallWeight;
    weights[4] /= overallWeight;
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
    masks.assign(0.0f);
    masks[map] = 1.0f;
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
