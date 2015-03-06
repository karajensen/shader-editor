////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

struct Outputs
{
    float4 scene   : SV_TARGET0;
    float4 normal  : SV_TARGET1;
    float4 effects : SV_TARGET2;
};

Texture2DMS<float4,SAMPLES> SceneSampler : register(t0);
Texture2DMS<float4,SAMPLES> NormalSampler : register(t1);

float4 GetMultisampled(Texture2DMS<float4, SAMPLES> samplerName, int3 uvs)
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

    Outputs output;
    output.scene = GetMultisampled(SceneSampler, uvs);
    output.normal = GetMultisampled(NormalSampler, uvs);
    output.effects = float4(1.0, 0.0, 0.0, 1.0);
    return output;
}