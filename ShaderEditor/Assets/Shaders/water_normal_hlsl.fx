////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_normal_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float depthNear;
    float depthFar;
};

struct Attributes
{
    float4 position  : SV_POSITION;
    float depth      : TEXCOORD0;
    float3 normal    : NORMAL;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0,
                   float3 normal    : NORMAL)
{
    Attributes output;
    output.position = mul(viewProjection, position);
    output.normal = normal;

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    return float4(normalize(input.normal), input.depth);
}