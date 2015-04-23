////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_effects_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float finalMask;
    float sceneMask;
    float depthMask;
    float blurSceneMask;
    float depthOfFieldMask;
    float fogMask;
    float bloomMask;

    float bloomIntensity;
    float contrast;
    float saturation;
    float fadeAmount;
    float dofStart;
    float dofFade;
    float fogStart;
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

SamplerState Sampler;
Texture2D SceneSampler : register(t0);
Texture2D BlurSampler  : register(t1);
Texture2DMS<float4,SAMPLES> DepthSampler : register(t2);

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
    float4 scene = SceneSampler.Sample(Sampler, input.uvs);
    float4 blur = BlurSampler.Sample(Sampler, input.uvs);
    float3 postScene = scene.rgb;

    // Depth information
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);
    float depth = DepthSampler.Load(uvs, 0).r;

    // Depth of Field
    float dofEnd = dofStart - dofFade;
    float dofWeight = saturate((depth-dofEnd)*(1.0/(dofStart-dofEnd)));
    float3 depthOfField = blur.rgb * dofWeight;
    postScene *= (1.0 - dofWeight);
    postScene += depthOfField;

    // Fog
    float fogEnd = fogStart + fogFade;
    float fogWeight = saturate((depth-fogEnd)*(1.0/(fogStart-fogEnd)));
    float3 fog = fogColor * fogWeight;
    postScene *= (1.0 - fogWeight);
    postScene += fog;

    // Bloom
    float3 bloom = blur.rgb * blur.a * bloomIntensity;
    postScene += bloom;

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
    output.colour.rgb += depth * depthMask;
    output.colour.rgb += blur.rgb * blurSceneMask;
    output.colour.rgb += depthOfField * depthOfFieldMask;
    output.colour.rgb += fog * fogMask;
    output.colour.rgb += bloom * bloomMask;
    output.colour.rgb *= fadeAmount;
    return output;
}