////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    ifdef: SPECULAR
        float3 cameraPosition;  
    endif
};

cbuffer MeshVertexBuffer : register(b1)
{
    float4x4 world;
};

cbuffer ScenePixelBuffer : register(b2)
{
    float lightActive[MAX_LIGHTS];
    float3 lightPosition[MAX_LIGHTS];
    float3 lightAttenuation[MAX_LIGHTS];
    float3 lightDiffuse[MAX_LIGHTS];
    ifdef: !FLAT|SPECULAR
        float lightSpecularity[MAX_LIGHTS];
        float3 lightSpecular[MAX_LIGHTS];
    endif
};

cbuffer MeshPixelBuffer : register(b3)
{
    float meshAmbience;
    float3 meshColour;
    ifdef: GLOW
        float meshGlow;
    endif
    ifdef: !FLAT|BUMP
        float meshBump;
    endif
    ifdef: !FLAT|SPECULAR
        float meshSpecularity;
    endif
};

SamplerState Sampler;
Texture2D DiffuseTexture;
Texture2D NormalTexture;
Texture2D SpecularTexture;
Texture2D GlowTexture;

struct Attributes
{
    float4 position                 : SV_POSITION;
    float2 uvs                      : TEXCOORD0;
    float3 positionWorld            : TEXCOORD1;
    ifdef: !FLAT                    
        float3 normal               : NORMAL;
        ifdef: BUMP
            float3 tangent          : TEXCOORD2;
            float3 bitangent        : TEXCOORD3;
        endif
        ifdef: SPECULAR|BUMP             
            float3 vertToCamera     : TEXCOORD4;
        elseif: SPECULAR
            float3 vertToCamera     : TEXCOORD2;
        endif
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
    output.uvs = uvs;
    output.positionWorld = mul(world, position).xyz;
    
    ifdef: !FLAT
        output.normal = mul(world, normal);
        ifdef: BUMP
            output.tangent = mul(world, tangent);
            output.bitangent = mul(world, bitangent);
        endif

        ifdef: SPECULAR
            output.vertToCamera = cameraPosition - output.positionWorld;
        endif
    endif

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs);
    float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);

    ifdef: !FLAT
        float3 normal = normalize(input.normal);
        ifdef: BUMP
            float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
            float2 bump = meshBump * (normalTex.rg - 0.5);
            normal = normalize(normal + bump.x * 
                normalize(input.tangent) + bump.y * normalize(input.bitangent));
        endif
    endif

    ifdef: !FLAT|SPECULAR
        float3 vertToCamera = normalize(input.vertToCamera);
        float4 specularTex = SpecularTexture.Sample(Sampler, input.uvs);
        float4 specular = float4(0.0, 0.0, 0.0, 0.0);
    endif

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        float3 lightColour = lightDiffuse[i];
        float3 vertToLight = lightPosition[i] - input.positionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        ifdef: !FLAT
            vertToLight /= lightLength;
            lightColour *= ((dot(vertToLight, normal) + 1.0) * 0.5);

            ifdef: SPECULAR
                float specularity = lightSpecularity[i] * meshSpecularity;
                float3 halfVector = normalize(vertToLight + vertToCamera);
                float specularFactor = pow(max(dot(normal, halfVector), 0.0), specularity); 
                specular.rgb += specularFactor * lightSpecular[i] * attenuation * lightActive[i];
            endif
        endif

        diffuse.rgb += lightColour * attenuation * lightActive[i];
    }

    float4 finalColour = diffuseTex * diffuse;
    ifdef: !FLAT|SPECULAR
        finalColour += specularTex * specular;
    endif
    finalColour.rgb *= meshAmbience;
    
    ifdef: GLOW
        finalColour.a = GlowTexture.Sample(Sampler, input.uvs).r * meshGlow;
    endif

    return finalColour;
}