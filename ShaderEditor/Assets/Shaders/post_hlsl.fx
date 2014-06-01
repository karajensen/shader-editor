
struct Attributes
{
    float4 position  : SV_POSITION;
    float2 uvs       : TEXCOORD0;
};

SamplerState Sampler;
Texture2D SceneTexture  : register(t0);

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
    return SceneTexture.Sample(Sampler, input.uvs);
}