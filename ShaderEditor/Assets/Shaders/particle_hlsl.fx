////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer MeshVertexBuffer : register(b0)
{
    float4x4 worldViewProjection;
};

cbuffer MeshPixelBuffer : register(b1)
{
    float alpha;
};

cbuffer ScenePixelBuffer : register(b2)
{
    float4 tint;
};

SamplerState Sampler;
Texture2D DiffuseTexture;

struct Attributes
{
    float4 position  : SV_POSITION;
    float2 uvs       : TEXCOORD0;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = mul(worldViewProjection, position);
    output.uvs = uvs;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 colour = DiffuseTexture.Sample(Sampler, input.uvs) * tint;
    colour.a *= alpha;
    return colour;
}