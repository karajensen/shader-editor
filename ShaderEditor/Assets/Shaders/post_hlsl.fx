////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float fadeAmount;
    float sceneAlpha;
    float normalAlpha;
    float depthAlpha;
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

Attributes VShader(float4 position  : POSITION,
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = position;
    output.uvs = uvs;
    return output;
}

float4 GetColour(Texture2DMS<float4, SAMPLES> texSampler, int3 uvs)
{
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += texSampler.Load(uvs, i);
    }
    return color * (1.0 / SAMPLES);
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColor;
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);

    if (sceneAlpha == 1.0)
    {
        finalColor = GetColour(SceneTexture, uvs);
        finalColor.rgb *= maximumColor - minimumColor;
        finalColor.rgb += minimumColor;
    }
    else
    {
        finalColor = GetColour(NormalTexture, uvs);
        finalColor = (finalColor * normalAlpha) + (finalColor.aaaa * depthAlpha);
    }

    return finalColor * fadeAmount;
}