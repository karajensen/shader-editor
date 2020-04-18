
cbuffer ScenePixelBuffer : register(b0)
{
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
};

Texture2DMS<float4,4> SceneSampler  : register(t0);

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

    int3 uvs = int3(input.uvs.x * 800, input.uvs.y * 600, 0);
    output.scene = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; ++i)
    {
        output.scene += SceneSampler.Load(uvs, i);
    }
    output.scene *= (1.0 / 4);

    float3 bloom = float3(bloomStart, bloomStart - bloomFade, 1.0);
    bloom = (output.scene.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    bloom = saturate(bloom);
    output.scene.a = max(bloom.r, max(bloom.b, bloom.g));

    return output;
}
