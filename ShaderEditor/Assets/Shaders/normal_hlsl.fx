
cbuffer VertexBuffer
{
    float4x4 viewProjection;
    float2 frustum;
};

struct Attributes
{
    float4 position  : SV_POSITION;
    float3 normal    : NORMAL;
    float depth      : TEXCOORD0;
};

Attributes VShader(float4 position    : POSITION,    
                   float2 uvs         : TEXCOORD0,
                   float3 normal      : NORMAL)
{
    Attributes output;
    output.position = mul(viewProjection, position);
    output.normal = normal;

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z-frustum.x)*
        ((depthBounds.x-depthBounds.y)/(frustum.y-frustum.x)))+depthBounds.y;

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    return float4(normalize(input.normal), input.depth);
}