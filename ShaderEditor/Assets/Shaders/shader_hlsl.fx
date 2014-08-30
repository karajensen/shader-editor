////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float3 lightPosition;
    ifdef: !FLAT|SPECULAR
        float3 cameraPosition;
    endif
};

cbuffer ScenePixelBuffer : register(b1)
{
    float3 lightAttenuation;
    float3 lightDiffuse;
    ifdef: !FLAT|SPECULAR
        float lightSpecularity;
        float3 lightSpecular;
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
    float3 vertToLight              : TEXCOORD1;
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
    output.vertToLight = lightPosition - position.xyz;
    
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
    float4 finalColour = DiffuseTexture.Sample(Sampler, input.uvs);
    finalColour.rgb *= lightDiffuse;
    
    float lightLen = length(input.vertToLight);
    float attenuation = 1.0 / (lightAttenuation.x 
        + lightAttenuation.y * lightLen 
        + lightAttenuation.z * lightLen * lightLen);
    float3 vertToLight = input.vertToLight / lightLen;

    ifdef: !FLAT

        float3 normal = normalize(input.normal);
        ifdef: BUMP
            float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
            float2 bump = meshBump * (normalTex.rg - 0.5);
            normal = normalize(normal + bump.x * normalize(input.tangent) + bump.y * normalize(input.bitangent));
        endif
        
        finalColour.rgb *= ((dot(vertToLight, normal) + 1.0) * 0.5);
                
        ifdef: SPECULAR
            float specularity = lightSpecularity * meshSpecularity;
            float4 specularTex = SpecularTexture.Sample(Sampler, input.uvs);
            float3 halfVector = normalize(vertToLight + normalize(input.vertToCamera));
            float specular = pow(max(dot(normal, halfVector), 0.0), specularity); 
            finalColour.rgb += specular * specularTex.rgb * lightSpecular;
        endif

    endif

    return finalColour * meshAmbience * attenuation;
}