//////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_hlsl.fx
// Reference: http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html#Ocean
/////////////////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float timer;
};

cbuffer MeshVertexBuffer : register(b1)
{
    float meshBump;
    float2 waveBumpSpeed;
    float2 waveOffset;
    float waveFrequency;
    float waveAmplitude;
};

cbuffer MeshPixelBuffer : register(b2)
{
    float3 deepColor;
    float3 shallowColor;
    float3 reflectionTint;
    float reflectionIntensity;
    float fresnalFactor;
};

SamplerState Sampler;
Texture2D DiffuseTexture;
Texture2D NormalTexture;

struct Wave 
{
  float frequency;  // 2*PI / wavelength
  float phase;      // speed * frequency
  float amplitude;
  float2 direction;
};

struct Attributes
{
    float4 position         : SV_POSITION;
    float3 normal           : NORMAL;
    float2 uvs              : TEXCOORD0;
    float3 positionWorld    : TEXCOORD1;
    float3 tangent          : TEXCOORD2;
    float3 bitangent        : TEXCOORD3;
};

Attributes VShader(float4 position    : POSITION,    
                   float2 uvs         : TEXCOORD0,
                   float3 normal      : NORMAL,
                   float3 tangent     : TEXCOORD1,
                   float3 bitangent   : TEXCOORD2)
{
    Attributes output;
    output.position = mul(viewProjection, position);
    output.uvs = uvs;
    output.positionWorld = position.xyz;
    output.normal = normal;
    output.tangent = tangent;
    output.bitangent = bitangent;

    Wave wave1;
    Wave wave2;




    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 colour;

    float4 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs);

    float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
    float2 bump = meshBump * (normalTex.rg - 0.5);
    float3 normal = normalize(normalize(input.normal) + bump.x * 
        normalize(input.tangent) + bump.y * normalize(input.bitangent));

    colour.a = min(0.0, diffuseTex.a);
    colour.rgb = normalTex.rgb;
    
    return colour;
}