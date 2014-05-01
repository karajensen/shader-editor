cbuffer ConstantBuffer
{
    float4x4 viewProjection;
    float testing;
}

struct VertexOutput
{
    float4 position  :SV_POSITION;
    float2 uvs       :TEXCOORD0;
    float3 normal    :NORMAL;
};

VertexOutput VShader(float4 position  :POSITION, 
                     float2 uvs       :TEXCOORD0,
                     float3 normal    :NORMAL)
{
    VertexOutput output;

    output.position = mul(viewProjection, position);
    output.normal = normal;
    output.uvs = uvs;
    output.uvs.x = testing;
    return output;
}

float4 PShader(VertexOutput input) : SV_TARGET
{
    return float4(input.normal,1.0);
}