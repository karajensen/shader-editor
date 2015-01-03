//////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_hlsl.fx
// Reference: http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html#Ocean
/////////////////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float3 cameraPosition;
    float timer;
};

cbuffer ScenePixelBuffer : register(b1)
{
    float3 lightPosition[MAX_LIGHTS];
    float3 lightAttenuation[MAX_LIGHTS];
    float3 lightDiffuse[MAX_LIGHTS];
    float3 lightSpecular[MAX_LIGHTS];
    float lightSpecularity[MAX_LIGHTS];
};

cbuffer MeshVertexBuffer : register(b2)
{
    float2 bumpVelocity;
    float2 uvScale;
    float speed;
    float waveFrequency[MAX_WAVES];
    float waveAmplitude[MAX_WAVES];
    float wavePhase[MAX_WAVES];
    float waveDirectionX[MAX_WAVES];
    float waveDirectionZ[MAX_WAVES];
};

cbuffer MeshPixelBuffer : register(b3)
{
    float3 deepColor;
    float fresnalFactor;
    float3 shallowColor;
    float bumpIntensity;
    float3 reflectionTint;
    float reflectionIntensity;
};

SamplerState Sampler;
Texture2D DiffuseTexture;
Texture2D NormalTexture;

struct Attributes
{
    float4 position         : SV_POSITION;
    float3 normal           : NORMAL;
    float2 uvs              : TEXCOORD0;
    float3 positionWorld    : TEXCOORD1;
    float3 tangent          : TEXCOORD2;
    float3 bitangent        : TEXCOORD3;
    float2 normalUV0        : TEXCOORD4;
    float2 normalUV1        : TEXCOORD5;
    float2 normalUV2        : TEXCOORD6;
};

Attributes VShader(float4 position    : POSITION,    
                   float2 uvs         : TEXCOORD0,
                   float3 normal      : NORMAL,
                   float3 tangent     : TEXCOORD1,
                   float3 bitangent   : TEXCOORD2)
{
    Attributes output;
    output.normal = normal;
    output.tangent = tangent;
    output.bitangent = bitangent;

    // Sum the waves together
    float time = timer * speed;
    float4 wavePosition = position;
    float2 waveDerivative = float2(0.0, 0.0);
    for (int i = 0; i < MAX_WAVES; i++)
    {
        // y = a * sin(kx-wt+phase)
        float2 direction = float2(waveDirectionX[i], waveDirectionZ[i]);
        float component = dot(direction, position.xz) - (waveFrequency[i] * time) + wavePhase[i];
        wavePosition.y += waveAmplitude[i] * sin(component);
        waveDerivative += waveFrequency[i] * waveAmplitude[i] * cos(component) * direction;
    }

    output.position = mul(viewProjection, wavePosition);
    output.positionWorld = wavePosition.xyz;

    // Generate normal map coordinates
    // TODO: Figure out what the magic numbers are doing
    float cycle = fmod(timer, 100.0);
    output.uvs = uvs * uvScale;
    output.normalUV0 = uvs * uvScale + cycle * bumpVelocity;
    output.normalUV1 = uvs * uvScale * 2.0 + cycle * bumpVelocity * 4.0;
    output.normalUV2 = uvs * uvScale * 4.0 + cycle * bumpVelocity * 8.0;

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 colour = DiffuseTexture.Sample(Sampler, input.uvs);

    float4 normalTex0 = NormalTexture.Sample(Sampler, input.normalUV0) * 2.0 - 1.0;
    float4 normalTex1 = NormalTexture.Sample(Sampler, input.normalUV1) * 2.0 - 1.0;
    float4 normalTex2 = NormalTexture.Sample(Sampler, input.normalUV2) * 2.0 - 1.0;
    float3 normalTex = normalTex0.xyz + normalTex1.xyz + normalTex2.xyz;
    
    return colour;
}