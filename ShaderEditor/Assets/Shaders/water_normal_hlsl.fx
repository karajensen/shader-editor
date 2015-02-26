////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_normal_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float depthNear;
    float depthFar;
    float timer;
};

cbuffer MeshVertexBuffer : register(b1)
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

cbuffer MeshPixelBuffer : register(b2)
{
    float bumpIntensity;
};

SamplerState Sampler;
Texture2D NormalTexture;

struct Attributes
{
    float4 position         : SV_POSITION;
    float3 normal           : NORMAL;
    float  depth            : TEXCOORD0;
    float3 bitangent        : TEXCOORD1;
    float3 tangent          : TEXCOORD2;
    float2 normalUV0        : TEXCOORD3;
    float2 normalUV1        : TEXCOORD4;
    float2 normalUV2        : TEXCOORD5;
};

Attributes VShader(float4 position  : POSITION,    
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;

    // Sum the waves together
    float time = timer * speed;
    float4 wavePosition = position;
    float2 waveDerivative = float2(0.0, 0.0);
    for (int i = 0; i < MAX_WAVES; i++)
    {
        // Wave equation: y = a * sin(kx-wt+phase)
        float2 direction = float2(waveDirectionX[i], waveDirectionZ[i]);
        float component = dot(direction, position.xz) - (waveFrequency[i] * time) + wavePhase[i];
        wavePosition.y += waveAmplitude[i] * sin(component);
        waveDerivative += waveFrequency[i] * waveAmplitude[i] * cos(component) * direction;
    }

    output.position = mul(viewProjection, wavePosition);
    output.bitangent = float3(1, waveDerivative.x, 0);
    output.tangent = float3(0, waveDerivative.y, 1);
    output.normal = float3(-waveDerivative.x, 1, -waveDerivative.y);

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;

    // Generate UV Coordinates
    float4 scale = float4(2.0, 4.0, 8.0, 0.001);
    float2 uvVelocity = bumpVelocity * timer * scale.w;
    output.normalUV0 = uvs * uvScale + uvVelocity;
    output.normalUV1 = uvs * uvScale * scale.x + uvVelocity * scale.y;
    output.normalUV2 = uvs * uvScale * scale.y + uvVelocity * scale.z;

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float3 normalTex0 = NormalTexture.Sample(Sampler, input.normalUV0).rgb - 0.5;
    float3 normalTex1 = NormalTexture.Sample(Sampler, input.normalUV1).rgb - 0.5;
    float3 normalTex2 = NormalTexture.Sample(Sampler, input.normalUV2).rgb - 0.5;
    float3 bump = bumpIntensity * (normalTex0 + normalTex1 + normalTex2);
    
    float3 normal = normalize(input.normal);
    float3 bitangent = normalize(input.bitangent);
    float3 tangent = normalize(input.tangent);
    normal = normalize(normal + bump.x * tangent + bump.y * bitangent);    

    return float4(normal, input.depth);
}