cbuffer ConstantBuffer
{
    float4x4 viewProjection;
    float3 lightPosition;
}

Texture2D DiffuseTexture : register(t0);
Texture2D SpecularTexture : register(t1);
Texture2D NormalTexture : register(t2);
SamplerState Sampler;

struct VertexOutput
{
    float4 position     :SV_POSITION;
    float2 uvs          :TEXCOORD0;
    float3 normal       :NORMAL;
    float3 vertToLight  :TEXCOORD1;
};

VertexOutput VShader(float4 position  :POSITION, 
                     float2 uvs       :TEXCOORD0,
                     float3 normal    :NORMAL)
{
    VertexOutput output;

    output.position = mul(viewProjection, position);
    output.normal = normal;
    output.uvs = uvs;
    output.vertToLight = normalize(lightPosition - position.xyz);

    return output;
}

float4 PShader(VertexOutput input) : SV_TARGET
{
    float4 finalColour;
    normalize(input.normal);

    float diffuse = (dot(input.vertToLight, input.normal) + 1.0) * 0.5; 
    
    float4 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs);
    float4 specularTex = SpecularTexture.Sample(Sampler, input.uvs);
    float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);

    finalColour = specularTex;
    finalColour.rgb *= diffuse;
    finalColour.a = 1.0;
    return finalColour;
}