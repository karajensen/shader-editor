////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float fadeAmount;
    float sceneAlpha;
    float normalAlpha;
    float depthAlpha;
    float glowAlpha;
    float blurGlowAlpha;
    float blurSceneAlpha;
    float3 minimumColor;
    float3 maximumColor;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

Texture2DMS<float4,SAMPLES> SceneTexture   : register(t0);
Texture2DMS<float4,SAMPLES> NormalTexture  : register(t1);

SamplerState Sampler;
Texture2D BlurTexture : register(t2);

float4 GetMultisampledColour(Texture2DMS<float4, SAMPLES> texSampler, int3 uvs)
{
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += texSampler.Load(uvs, i);
    }
    return color * (1.0 / SAMPLES);
}

Attributes VShader(float4 position  : POSITION,
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = position;
    output.uvs = uvs;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColor;
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);

    if (sceneAlpha == 1.0)
    {
        finalColor = GetMultisampledColour(SceneTexture, uvs);
        finalColor.rgb *= maximumColor - minimumColor;
        finalColor.rgb += minimumColor;
    }
    else if (glowAlpha == 1.0)
    {
        finalColor.rgb = GetMultisampledColour(SceneTexture, uvs).aaa;
    }
    else if (blurSceneAlpha == 1.0)
    {
        finalColor.rgb = BlurTexture.Sample(Sampler, input.uvs).rgb;
    }
    else if (blurGlowAlpha == 1.0)
    {
        finalColor.rgb = BlurTexture.Sample(Sampler, input.uvs).aaa;
    }
    else
    {
        finalColor = GetMultisampledColour(NormalTexture, uvs);
        finalColor.rgb = (finalColor.rgb * normalAlpha) + (finalColor.aaa * depthAlpha);
    }

    return finalColor * fadeAmount;
}