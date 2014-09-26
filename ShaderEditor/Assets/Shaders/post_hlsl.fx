////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float fadeAmount;
    float glowAmount;
    float dofDistance;
    float dofFade;
    float finalMask;
    float sceneMask;
    float normalMask;
    float depthMask;
    float glowMask;
    float blurGlowMask;
    float blurSceneMask;
    float depthOfFieldMask;
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
    float3 postScene = scene.rgb;
    float depth = normal.a;

    // Adding Depth of Field
    float3 depthOfField = float3(0,0,0);
    if (depth >= dofEnd)
    {
        float dofEnd = dofDistance - dofFade;
        float weight = saturate((depth-dofEnd)*(1.0/(dofDistance-dofEnd)));
        depthOfField = blur.rgb * weight;
        postScene *= (1.0 - weight);
        postScene += depthOfField;
    }

    // Adding Glow
    float3 postGlow = blur.a * glowAmount * blur.rgb * depth;
    postScene += postGlow;

    // Colour Correction
    saturate(postScene);
    postScene *= maximumColor - minimumColor;
    postScene += minimumColor;

    // Masking the selected texture
    float4 outColor;
    outColor.rgb = postScene * finalMask;
    outColor.rgb += scene.rgb * sceneMask;
    outColor.rgb += normal.rgb * normalMask;
    outColor.rgb += normal.aaa * depthMask;
    outColor.rgb += scene.aaa * glowMask;
    outColor.rgb += postGlow * blurGlowMask;
    outColor.rgb += blur.rgb * blurSceneMask;
    outColor.rgb += depthOfField * depthOfFieldMask;
    outColor.rgb *= fadeAmount;
    return outColor;
}