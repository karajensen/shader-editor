////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_vertical_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float blurStep;
    float weightMain;
    float4 weightOffset;
};

struct Attributes
{
    float4 position : SV_POSITION;
    float2 uvs      : TEXCOORD0;
};

struct Outputs
{
    float4 scene    : SV_TARGET0;
    float4 effects  : SV_TARGET1;
};

SamplerState Sampler;
Texture2D SceneSampler;
Texture2D EffectsSampler;

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
    Outputs output;

    float4 uvSteps = float4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    float2 uvs1p = float2(input.uvs.x, input.uvs.y + uvSteps.x);
    float2 uvs1n = float2(input.uvs.x, input.uvs.y - uvSteps.x);
    float2 uvs2p = float2(input.uvs.x, input.uvs.y + uvSteps.y);
    float2 uvs2n = float2(input.uvs.x, input.uvs.y - uvSteps.y);
    float2 uvs3p = float2(input.uvs.x, input.uvs.y + uvSteps.z);
    float2 uvs3n = float2(input.uvs.x, input.uvs.y - uvSteps.z);
    float2 uvs4p = float2(input.uvs.x, input.uvs.y + uvSteps.w);
    float2 uvs4n = float2(input.uvs.x, input.uvs.y - uvSteps.w);

    output.scene =  SceneSampler.Sample(Sampler, input.uvs) * weightMain;
    output.scene += SceneSampler.Sample(Sampler, uvs1p) * weightOffset.x;
    output.scene += SceneSampler.Sample(Sampler, uvs1n) * weightOffset.x;
    output.scene += SceneSampler.Sample(Sampler, uvs2p) * weightOffset.y;
    output.scene += SceneSampler.Sample(Sampler, uvs2n) * weightOffset.y;
    output.scene += SceneSampler.Sample(Sampler, uvs3p) * weightOffset.z;
    output.scene += SceneSampler.Sample(Sampler, uvs3n) * weightOffset.z;
    output.scene += SceneSampler.Sample(Sampler, uvs4p) * weightOffset.w;
    output.scene += SceneSampler.Sample(Sampler, uvs4n) * weightOffset.w;

    output.effects =  EffectsSampler.Sample(Sampler, input.uvs) * weightMain;
    output.effects += EffectsSampler.Sample(Sampler, uvs1p) * weightOffset.x;
    output.effects += EffectsSampler.Sample(Sampler, uvs1n) * weightOffset.x;
    output.effects += EffectsSampler.Sample(Sampler, uvs2p) * weightOffset.y;
    output.effects += EffectsSampler.Sample(Sampler, uvs2n) * weightOffset.y;
    output.effects += EffectsSampler.Sample(Sampler, uvs3p) * weightOffset.z;
    output.effects += EffectsSampler.Sample(Sampler, uvs3n) * weightOffset.z;
    output.effects += EffectsSampler.Sample(Sampler, uvs4p) * weightOffset.w;
    output.effects += EffectsSampler.Sample(Sampler, uvs4n) * weightOffset.w;

    return output;
}