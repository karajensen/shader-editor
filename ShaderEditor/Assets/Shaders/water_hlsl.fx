////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
};

struct Attributes
{
    float4 position       : SV_POSITION;
    float2 uvs            : TEXCOORD0;
};

Attributes VShader(float4 position      : POSITION,    
                   float2 uvs           : TEXCOORD0)
{
    Attributes output;
    output.position = mul(viewProjection, position);
    output.uvs = uvs;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    return float4(1.0, 0.0, 1.0, 1.0);
}