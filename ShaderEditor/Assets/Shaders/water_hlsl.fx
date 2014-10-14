////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
};

SamplerState Sampler;
Texture2D DiffuseTexture;

struct Attributes
{
    float4 position  : SV_POSITION;
    float2 uvs       : TEXCOORD0;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0,
                   float3 normal    : NORMAL)
{
    Attributes output;
    output.position = mul(viewProjection, position);
    output.uvs = uvs;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    return float4(DiffuseTexture.Sample(Sampler, input.uvs).rgb, 0.0);
}