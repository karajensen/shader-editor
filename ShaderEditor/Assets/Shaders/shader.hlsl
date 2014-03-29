cbuffer ConstantBuffer
{
    float4x4 viewProjection;
    float testing;
}

struct VertexOutput
{
    float4 position :SV_POSITION;
    float4 color    :COLOR;
};

VertexOutput VShader(float4 position :POSITION, 
                     float4 color    :COLOR)
{
    VertexOutput output;

    output.position = mul(viewProjection, position);
    output.color = color;
    output.color.g = testing;

    return output;
}

float4 PShader(float4 position :SV_POSITION, 
               float4 color    :COLOR) : SV_TARGET
{
    return color;
}