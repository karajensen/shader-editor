////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    ifdef: !FLAT|SPECULAR
        float3 cameraPosition;
    endif
};

cbuffer ScenePixelBuffer : register(b1)
{
    float3 lightPosition[MAX_LIGHTS];
    float3 lightAttenuation[MAX_LIGHTS];
    float3 lightDiffuse[MAX_LIGHTS];
    ifdef: !FLAT|SPECULAR
        float lightSpecularity[MAX_LIGHTS];
        float3 lightSpecular[MAX_LIGHTS];
    endif
};

cbuffer MeshPixelBuffer : register(b2)
{
    float meshAmbience;
    ifdef: !FLAT|BUMP
        float meshBump;
    endif
    ifdef: !FLAT|SPECULAR
        float meshSpecularity;
    endif
};

// Required in order of usage in shader body
SamplerState Sampler;
Texture2D DiffuseTexture          : register(t0);
ifdef: !FLAT
    ifdef: SPECULAR|BUMP
        Texture2D NormalTexture   : register(t1);
        Texture2D SpecularTexture : register(t2);
    elseif: SPECULAR
        Texture2D SpecularTexture : register(t1);
    elseif: BUMP
        Texture2D NormalTexture   : register(t1);
    endif
endif

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

    output.position = mul(viewProjection, position);
    output.uvs = uvs;
    output.positionWorld = position.xyz;
    
    ifdef: !FLAT
        output.normal = normal;
        ifdef: BUMP
            output.tangent = tangent;
            output.bitangent = bitangent;
        endif

        ifdef: SPECULAR
            output.vertToCamera = cameraPosition - position.xyz;
        endif
    endif

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 diffuseTex = DiffuseTexture.Sample(Sampler, input.uvs);
    float3 diffuse = float3(0.0, 0.0, 0.0);

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
        float3 specular = float3(0.0, 0.0, 0.0);
    endif

    //for (int i = 0; i < MAX_LIGHTS; ++i)
    int i = 1;
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
                specular += specularFactor * lightSpecular[i] * attenuation;
            endif
        endif

        diffuse += lightColour * attenuation;
    }

    float3 finalColour = diffuseTex * diffuse;
    ifdef: !FLAT|SPECULAR
        finalColour += specularTex * specular;
    endif
    return float4(finalColour * meshAmbience, 1.0);
}