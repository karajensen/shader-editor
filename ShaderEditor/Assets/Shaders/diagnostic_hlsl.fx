////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
};

cbuffer MeshVertexBuffer : register(b1)
{
    float4x4 world;
};

cbuffer MeshPixelBuffer : register(b2)
{
    float3 meshColour;
};

struct Attributes
{
    float4 position        : SV_POSITION;
    float3 normal          : NORMAL;
    float3 positionWorld   : TEXCOORD0;
};

Attributes VShader(float4 position : POSITION,
                   float3 normal   : NORMAL)
{
    Attributes output;
    output.position = mul(mul(viewProjection, world), position);
    output.normal = mul(world, normal);
    output.positionWorld = mul(world, position);
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float3 vertToLight = float3(DIAGNOSTIC_LIGHT) - input.positionWorld;
    float diffuse = ((dot(normalize(vertToLight), normalize(input.normal))+1.0)*0.5);
    return float4(meshColour * diffuse, 0.0);
}