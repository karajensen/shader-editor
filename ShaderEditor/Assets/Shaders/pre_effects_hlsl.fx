////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer ScenePixelBuffer : register(b0)
{
    float bloomIntensity;
    float bloomStart;
    float bloomFade;
};

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

SamplerState Sampler;
Texture2DMS<float4,SAMPLES> SceneSampler  : register(t0);
Texture2DMS<float4,SAMPLES> NormalSampler : register(t1);
Texture2D RandomSampler : register(t2);

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
    float4 scene = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        scene += SceneSampler.Load(uvs, i);
    }
    scene *= (1.0 / SAMPLES);

    Outputs output;
    output.scene = scene;
    output.normal = NormalSampler.Load(uvs, 0);

    // Create the Bloom
    // map range from end->start to 0->1
    float3 bloom = float3(bloomStart, bloomStart - bloomFade, 1.0);
    output.effects.rgb = (scene.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    output.effects.rgb = saturate(output.effects.rgb);
    output.effects.rgb *= bloomIntensity;

    // Ambient Occlusion
    float4 random = RandomSampler.Sample(Sampler, input.uvs);
    output.effects.a = 1.0;

    return output;
}