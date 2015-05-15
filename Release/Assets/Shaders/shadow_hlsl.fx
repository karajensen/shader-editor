////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadow_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
};

cbuffer MeshVertexBuffer : register(b1)
{
    float4x4 world;
};

SamplerState Sampler;
Texture2D DiffuseTexture;

struct Attributes
{
    float4 position  : SV_POSITION;
    float2 uvs       : TEXCOORD0;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = mul(mul(viewProjection, world), position);
    output.uvs = uvs;
    return output;
}

Outputs PShader(Attributes input)
{
    Outputs output;
    output.colour = DiffuseTexture.Sample(Sampler, input.uvs);
    output.colour.a = 1.0;
    return output;
}