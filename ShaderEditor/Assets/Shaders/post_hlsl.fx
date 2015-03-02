////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float finalMask;
    float sceneMask;
    float normalMask;
    float depthMask;
    float blurSceneMask;
    float depthOfFieldMask;
    float fogMask;

    float contrast;
    float saturation;
    float fadeAmount;
    float dofDistance;
    float dofFade;
    float fogDistance;
    float fogFade;
    float3 fogColor;
    float3 minimumColor;
    float3 maximumColor;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
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

Outputs PShader(Attributes input)
{
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);
    float4 scene = GetMultisampledColour(SceneSampler, uvs);
    float4 normal = GetMultisampledColour(NormalSampler, uvs);
    float4 blur = BlurSampler.Load(uvs, 0);
    float3 postScene = scene.rgb;
    float depth = normal.a;

    // Depth of Field
    float dofEnd = dofDistance - dofFade;
    float dofWeight = saturate((depth-dofEnd)*(1.0/(dofDistance-dofEnd)));
    float3 depthOfField = blur.rgb * dofWeight;
    postScene *= (1.0 - dofWeight);
    postScene += depthOfField;

    // Fog
    float fogEnd = fogDistance + fogFade;
    float fogWeight = saturate((depth-fogEnd)*(1.0/(fogDistance-fogEnd)));
    float3 fog = fogColor * fogWeight;
    postScene *= (1.0 - fogWeight);
    postScene += fog;

    // Colour Correction
    postScene = saturate(postScene);
    postScene *= maximumColor - minimumColor;
    postScene += minimumColor;

    // Contrast and saturation
    // Reference: Programming vertex, geometry and pixel shaders p378-379
    float luminance = dot(postScene.rgb, float3(0.2126, 0.7152, 0.0722));
    postScene.rgb = lerp(float3(luminance, luminance, luminance), postScene.rgb, saturation);
    postScene -= contrast * (postScene - 1.0) * postScene * (postScene - 0.5);

    // Masking the selected texture
    Outputs output;
    output.colour.rgb = postScene * finalMask;
    output.colour.rgb += scene.rgb * sceneMask;
    output.colour.rgb += normal.rgb * normalMask;
    output.colour.rgb += normal.aaa * depthMask;
    output.colour.rgb += blur.rgb * blurSceneMask;
    output.colour.rgb += depthOfField * depthOfFieldMask;
    output.colour.rgb += fog * fogMask;
    output.colour.rgb *= fadeAmount;
    output.colour.a = 1.0;
    return output;
}