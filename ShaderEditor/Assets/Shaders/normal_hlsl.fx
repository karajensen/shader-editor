////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float depthNear;
    float depthFar;
};

cbuffer MeshVertexBuffer : register(b1)
{
    float4x4 world;
};

ifdef: BUMP
cbuffer MeshPixelBuffer : register(b2)
{
    float meshBump;
};

SamplerState Sampler;
Texture2D NormalTexture : register(t0);
endif

struct Attributes
{
    float4 position       : SV_POSITION;
    float3 normal         : NORMAL;
    float depth           : TEXCOORD0;
    ifdef: BUMP
        float2 uvs        : TEXCOORD1;
        float3 tangent    : TEXCOORD2;
        float3 bitangent  : TEXCOORD3;
    endif
};

Attributes VShader(float4 position      : POSITION,    
                   float2 uvs           : TEXCOORD0,
                   ifdef: BUMP
                       float3 normal    : NORMAL,
                       float3 tangent   : TEXCOORD1,
                       float3 bitangent : TEXCOORD2)
                   else:
                       float3 normal    : NORMAL)
                   endif
{
    Attributes output;
    output.position = mul(mul(viewProjection, world), position);
    output.normal = mul(world, normal);

    ifdef: BUMP
        output.uvs = uvs;
        output.tangent = mul(world, tangent);
        output.bitangent = mul(world, bitangent);
    endif

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    ifdef: BUMP
        float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
        float2 bump = meshBump * (normalTex.rg - 0.5);
        normal = normalize(normal + bump.x * normalize(input.tangent) + bump.y * normalize(input.bitangent));
    endif

    return float4(normal, input.depth);
}