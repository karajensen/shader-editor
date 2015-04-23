////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_hlsl.fx
// Based on nvidia Water Shader: http://nvidia.com/shaderlibrary
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float3 cameraPosition;
    float depthNear;
    float depthFar;
    float timer;
};

cbuffer ScenePixelBuffer : register(b1)
{
    float3 lightPosition[MAX_LIGHTS];
    float3 lightAttenuation[MAX_LIGHTS];
    float3 lightDiffuse[MAX_LIGHTS];
    float lightActive[MAX_LIGHTS];
};

cbuffer MeshVertexBuffer : register(b2)
{
    float4x4 world;
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
    float4 deepColor;
    float4 shallowColor;
    float3 reflectionTint;
    float reflectionIntensity;
    float3 fresnal;
    float bumpIntensity;
};

SamplerState Sampler;
Texture2D DiffuseTexture;
Texture2D NormalTexture;
TextureCube EnvironmentTexture;

struct Attributes
{
    float4 position         : SV_POSITION;
    float3 normal           : NORMAL;
    float  depth            : TEXCOORD0;
    float2 uvs              : TEXCOORD1;
    float3 positionWorld    : TEXCOORD2;
    float3 bitangent        : TEXCOORD3;
    float3 tangent          : TEXCOORD4;
    float2 normalUV0        : TEXCOORD5;
    float2 normalUV1        : TEXCOORD6;
    float2 normalUV2        : TEXCOORD7;
    float3 vertToCamera     : TEXCOORD8;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
    float4 depth  : SV_TARGET1;
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

    wavePosition = mul(world, wavePosition);
    output.position = mul(viewProjection, wavePosition);
    output.positionWorld = wavePosition.xyz;
    output.bitangent = float3(1, waveDerivative.x, 0);
    output.tangent = float3(0, waveDerivative.y, 1);
    output.normal = float3(-waveDerivative.x, 1, -waveDerivative.y);
    output.vertToCamera = cameraPosition - output.positionWorld;

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;

    // Generate UV Coordinates
    float4 scale = float4(2.0, 4.0, 8.0, 0.001);
    float2 uvVelocity = bumpVelocity * timer * scale.w;
    output.uvs = uvs * uvScale;
    output.normalUV0 = uvs * uvScale + uvVelocity;
    output.normalUV1 = uvs * uvScale * scale.x + uvVelocity * scale.y;
    output.normalUV2 = uvs * uvScale * scale.y + uvVelocity * scale.z;

    return output;
}

Outputs PShader(Attributes input)
{
    float3 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs).rgb;
    float3 diffuse = float3(0.0, 0.0, 0.0);
    
    float3 normalTex0 = NormalTexture.Sample(Sampler, input.normalUV0).rgb - 0.5;
    float3 normalTex1 = NormalTexture.Sample(Sampler, input.normalUV1).rgb - 0.5;
    float3 normalTex2 = NormalTexture.Sample(Sampler, input.normalUV2).rgb - 0.5;
    float3 bump = bumpIntensity * (normalTex0 + normalTex1 + normalTex2);
    
    float3 normal = normalize(input.normal);
    float3 bitangent = normalize(input.bitangent);
    float3 tangent = normalize(input.tangent);
    normal = normalize(normal + bump.x * tangent + bump.y * bitangent);
    
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        float3 lightColour = lightDiffuse[i];
        float3 vertToLight = lightPosition[i] - input.positionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);
    
        vertToLight /= lightLength;
        lightColour *= ((dot(vertToLight, normal) + 1.0) * 0.5);
        diffuse += lightColour * attenuation * lightActive[i];
    }
    
    // Fresnal Approximation = max(0, min(1, bias + scale * pow(1.0 + dot(I,N))))
    // Reference: NVIDEA CG Chapter 7 Environment Mapping Techniques
    float3 vertToCamera = normalize(input.vertToCamera);
    float fresnalFactor = saturate(fresnal.x + fresnal.y * pow(1.0 + dot(-vertToCamera, normal), fresnal.z));
    float3 reflection = reflect(-vertToCamera, normal);
    float4 reflectionTex = EnvironmentTexture.Sample(Sampler, reflection);
    
    Outputs output;
    output.depth = float4(input.depth, input.depth, input.depth, 1.0);

    output.colour.rgb = diffuseTex * diffuse;
    output.colour.a = 1.0;
    output.colour *= (saturate(dot(vertToCamera, normal))*(deepColor-shallowColor))+shallowColor;
    output.colour.rgb += reflectionTex.rgb * reflectionTint * reflectionIntensity * fresnalFactor;

    return output;
}