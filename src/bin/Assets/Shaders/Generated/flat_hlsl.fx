
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

cbuffer MeshPixelBuffer : register(b2)
{
    float meshAmbience;
};

cbuffer ScenePixelBuffer : register(b3)
{
};

SamplerState Sampler;
Texture2D DiffuseSampler;

struct Attributes
{
    float4 position          : SV_POSITION;
    float  depth             : TEXCOORD0;
    float2 uvs               : TEXCOORD1;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
    float4 depth  : SV_TARGET1;
};

Attributes VShader(float4 position      : POSITION,    
                   float2 uvs           : TEXCOORD0)
{
    Attributes output;

    output.position = mul(mul(viewProjection, world), position);
    output.uvs = uvs;

    output.depth = ((output.position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;
    
    return output;
}

Outputs PShader(Attributes input)
{
    float4 diffuseTex = DiffuseSampler.Sample(Sampler, input.uvs);

    Outputs output;
    output.depth = float4(input.depth, input.depth, input.depth, 1.0);

    output.colour.rgb = diffuseTex.rgb;
    output.colour.rgb *= meshAmbience;
    output.colour.a = 1.0;

    return output;
}
