
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

    output.scene =  SceneSampler.Sample(Sampler, input.uvs) * 0.224215;
    output.scene += SceneSampler.Sample(Sampler, uvs1p) * 0.201794;
    output.scene += SceneSampler.Sample(Sampler, uvs1n) * 0.201794;
    output.scene += SceneSampler.Sample(Sampler, uvs2p) * 0.123318;
    output.scene += SceneSampler.Sample(Sampler, uvs2n) * 0.123318;
    output.scene += SceneSampler.Sample(Sampler, uvs3p) * 0.040359;
    output.scene += SceneSampler.Sample(Sampler, uvs3n) * 0.040359;
    output.scene += SceneSampler.Sample(Sampler, uvs4p) * 0.022422;
    output.scene += SceneSampler.Sample(Sampler, uvs4n) * 0.022422;

    return output;
}
