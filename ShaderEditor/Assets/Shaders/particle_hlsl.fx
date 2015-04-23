////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer MeshVertexBuffer : register(b0)
{
    float4x4 worldViewProjection;
    float depthNear;
    float depthFar;
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
    float depth      : TEXCOORD1;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
    float4 depth  : SV_TARGET1;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = mul(worldViewProjection, position);
    output.uvs = uvs;

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;

    return output;
}

Outputs PShader(Attributes input)
{
    Outputs output;
    output.colour = DiffuseTexture.Sample(Sampler, input.uvs) * tint;
    output.colour.a *= alpha;
    output.depth = float4(input.depth, input.depth, input.depth, output.colour.a);
    return output;
}