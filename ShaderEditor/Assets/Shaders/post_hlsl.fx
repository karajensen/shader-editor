////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float fadeAmount;
    float glowAmount;
    float finalMask;
    float sceneMask;
    float normalMask;
    float depthMask;
    float glowMask;
    float blurGlowMask;
    float blurSceneMask;
    float3 minimumColor;
    float3 maximumColor;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

Texture2DMS<float4,SAMPLES> SceneSampler   : register(t0);
Texture2DMS<float4,SAMPLES> NormalSampler  : register(t1);
Texture2DMS<float4,SAMPLES> BlurSampler    : register(t2);

float4 GetMultisampledColour(Texture2DMS<float4, SAMPLES> samplerName, int3 uvs)
{
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += samplerName.Load(uvs, i);
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
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);
    float4 scene = GetMultisampledColour(SceneSampler, uvs);
    float4 normal = GetMultisampledColour(NormalSampler, uvs);
    float4 blur = GetMultisampledColour(BlurSampler, uvs);
    float depth = normal.a;
    float3 postScene = scene.rgb;

    // Adding glow
    float3 postGlow = blur.a * glowAmount * blur.rgb * depth;
    postScene.rgb += postGlow;

    // Colour correction
    postScene.rgb *= maximumColor - minimumColor;
    postScene.rgb += minimumColor;

    // Masking the selected texture
    float4 finalColor = float4(0,0,0,0);
    finalColor.rgb += postScene * finalMask;
    finalColor.rgb += scene.rgb * sceneMask;
    finalColor.rgb += normal.rgb * normalMask;
    finalColor.rgb += normal.aaa * depthMask;
    finalColor.rgb += scene.aaa * glowMask;
    finalColor.rgb += postGlow * blurGlowMask;
    finalColor.rgb += blur.rgb * blurSceneMask;

    return finalColor * fadeAmount;
}