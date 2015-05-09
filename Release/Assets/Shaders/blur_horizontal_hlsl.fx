////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_horizontal_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float blurStep;
};

struct Attributes
{
    float4 position : SV_POSITION;
    float2 uvs      : TEXCOORD0;
};

struct Outputs
{
    float4 scene    : SV_TARGET0;
};                  

SamplerState Sampler;
Texture2D SceneSampler;

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
    float2 uvs1p = float2(input.uvs.x + uvSteps.x, input.uvs.y);
    float2 uvs1n = float2(input.uvs.x - uvSteps.x, input.uvs.y);
    float2 uvs2p = float2(input.uvs.x + uvSteps.y, input.uvs.y);
    float2 uvs2n = float2(input.uvs.x - uvSteps.y, input.uvs.y);
    float2 uvs3p = float2(input.uvs.x + uvSteps.z, input.uvs.y);
    float2 uvs3n = float2(input.uvs.x - uvSteps.z, input.uvs.y);
    float2 uvs4p = float2(input.uvs.x + uvSteps.w, input.uvs.y);
    float2 uvs4n = float2(input.uvs.x - uvSteps.w, input.uvs.y);

    output.scene =  SceneSampler.Sample(Sampler, input.uvs) * WEIGHT0;
    output.scene += SceneSampler.Sample(Sampler, uvs1p) * WEIGHT1;
    output.scene += SceneSampler.Sample(Sampler, uvs1n) * WEIGHT1;
    output.scene += SceneSampler.Sample(Sampler, uvs2p) * WEIGHT2;
    output.scene += SceneSampler.Sample(Sampler, uvs2n) * WEIGHT2;
    output.scene += SceneSampler.Sample(Sampler, uvs3p) * WEIGHT3;
    output.scene += SceneSampler.Sample(Sampler, uvs3n) * WEIGHT3;
    output.scene += SceneSampler.Sample(Sampler, uvs4p) * WEIGHT4;
    output.scene += SceneSampler.Sample(Sampler, uvs4n) * WEIGHT4;

    return output;
}