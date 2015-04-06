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

struct Attributes
{
    float4 position        : SV_POSITION;
    float3 normal          : NORMAL;
    float3 positionWorld   : TEXCOORD0;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
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

Outputs PShader(Attributes input)
{
    float3 vertToLight = float3(DIAGNOSTIC_LIGHT) - input.positionWorld;
    float diffuse = ((dot(normalize(vertToLight), normalize(input.normal))+1.0)*0.5);

    Outputs output;
    output.colour.r = diffuse;
    output.colour.g = diffuse;
    output.colour.b = diffuse;
    output.colour.a = 0.0;
    return output;
}