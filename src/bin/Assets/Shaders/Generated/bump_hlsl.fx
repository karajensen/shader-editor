
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
    float meshDiffuse;
    float meshBump;
};

cbuffer ScenePixelBuffer : register(b3)
{
    float lightActive[1];
    float3 lightPosition[1];
    float3 lightAttenuation[1];
    float3 lightDiffuse[1];
};

SamplerState Sampler;
Texture2D DiffuseSampler;
Texture2D NormalSampler;

struct Attributes
{
    float4 position          : SV_POSITION;
    float  depth             : TEXCOORD0;
    float2 uvs               : TEXCOORD1;
    float3 normal            : NORMAL;
    float3 positionWorld     : TEXCOORD2;
    float3 tangent           : TEXCOORD3;
    float3 bitangent         : TEXCOORD4;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
    float4 depth  : SV_TARGET1;
};

Attributes VShader(float4 position      : POSITION,    
                   float2 uvs           : TEXCOORD0,
                   float3 normal        : NORMAL,
                   float3 tangent       : TEXCOORD1,
                   float3 bitangent     : TEXCOORD2)
{
    Attributes output;

    output.position = mul(mul(viewProjection, world), position);
    output.uvs = uvs;
    output.normal = mul(world, normal);
    output.positionWorld = mul(world, position).xyz;

    output.depth = ((output.position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;
    
    output.tangent = mul(world, tangent);
    output.bitangent = mul(world, bitangent);

    return output;
}

Outputs PShader(Attributes input)
{
    float4 diffuseTex = DiffuseSampler.Sample(Sampler, input.uvs);

    float3 diffuse = float3(0.0, 0.0, 0.0);
    float3 normal = normalize(input.normal);

    float4 normalTex = NormalSampler.Sample(Sampler, input.uvs);
    float2 bump = meshBump * (normalTex.rg - 0.5);
    normal = normalize(normal + bump.x * 
        normalize(input.tangent) + bump.y * normalize(input.bitangent));

    for (int i = 0; i < 1; ++i)
    {
        float3 lightColour = lightDiffuse[i];
        float3 vertToLight = lightPosition[i] - input.positionWorld;
        float lightLength = length(vertToLight);
    
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        vertToLight /= lightLength;

        lightColour *= ((dot(vertToLight, normal) + 1.0) *
            ((1.0 - meshDiffuse) * 0.5)) + meshDiffuse;

        diffuse += lightColour * attenuation * lightActive[i];

    }

    Outputs output;
    output.depth = float4(input.depth, input.depth, input.depth, 1.0);

    output.colour.rgb = diffuseTex.rgb * diffuse;
    output.colour.rgb *= meshAmbience;
    output.colour.a = 1.0;

    return output;
}
