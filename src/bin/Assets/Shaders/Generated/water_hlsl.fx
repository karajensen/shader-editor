
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
    float3 lightPosition[1];
    float3 lightAttenuation[1];
    float3 lightDiffuse[1];
    float lightActive[1];
};

cbuffer MeshVertexBuffer : register(b2)
{
    float4x4 world;
    float2 uvScale;
    float2 bumpScale;
    float speed;
    float waveFrequency[2];
    float waveAmplitude[2];
    float wavePhase[2];
    float waveDirectionX[2];
    float waveDirectionZ[2];
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
    float2 normalUV1        : TEXCOORD5;
    float2 normalUV2        : TEXCOORD6;
    float3 vertToCamera     : TEXCOORD7;
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

    float time = timer * speed;
    float4 wavePosition = position;
    float2 waveDerivative = float2(0.0, 0.0);
    for (int i = 0; i < 2; i++)
    {
        
        float2 direction = float2(waveDirectionX[i], waveDirectionZ[i]);
        float component = dot(direction, position.xz) - (waveFrequency[i] * time) + wavePhase[i];
        wavePosition.y += waveAmplitude[i] * sin(component);
        waveDerivative += waveFrequency[i] * waveAmplitude[i] * cos(component) * direction;
    }

    wavePosition = mul(world, wavePosition);
    output.position = mul(viewProjection, wavePosition);
    output.positionWorld = wavePosition.xyz;
    output.uvs = uvs * uvScale;
    output.normalUV1 = output.uvs * bumpScale.x;
    output.normalUV2 = output.uvs * bumpScale.y;
    output.bitangent = float3(1.0, waveDerivative.x, 0.0);
    output.tangent = float3(0.0, waveDerivative.y, 1.0);
    output.normal = float3(-waveDerivative.x, 1.0, -waveDerivative.y);
    output.vertToCamera = cameraPosition - output.positionWorld;

    output.depth = ((output.position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;

    return output;
}

Outputs PShader(Attributes input)
{
    float3 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs).rgb;
    float3 diffuse = float3(0.0, 0.0, 0.0);
    
    float3 normalTex0 = NormalTexture.Sample(Sampler, input.uvs).rgb - 0.5;
    float3 normalTex1 = NormalTexture.Sample(Sampler, input.normalUV1).rgb - 0.5;
    float3 normalTex2 = NormalTexture.Sample(Sampler, input.normalUV2).rgb - 0.5;
    float3 bump = bumpIntensity * (normalTex0 + normalTex1 + normalTex2);
    
    float3 normal = normalize(input.normal);
    float3 bitangent = normalize(input.bitangent);
    float3 tangent = normalize(input.tangent);
    normal = normalize(normal + bump.x * tangent + bump.y * bitangent);
    
    for (int i = 0; i < 1; ++i)
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
