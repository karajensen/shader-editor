////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    ifdef: !FLAT
        float3 lightPosition;
        ifdef: SPECULAR
            float3 cameraPosition;
        endif
    endif
};

cbuffer ScenePixelBuffer : register(b1)
{
    ifdef: !FLAT
        float3 lightAttenuation;
        float3 lightDiffuse;
        ifdef: SPECULAR
            float lightSpecularity;
            float3 lightSpecular;
        endif
    endif
};

cbuffer MeshPixelBuffer : register(b2)
{
    float meshAmbience;
    ifdef: !FLAT|SPECULAR
        float meshSpecularity;
    endif
};

SamplerState Sampler;
Texture2D DiffuseTexture          : register(t0);
ifdef: !FLAT
    ifdef: SPECULAR|BUMP
        Texture2D SpecularTexture : register(t1);
        Texture2D NormalTexture   : register(t2);
    elseif: SPECULAR
        Texture2D SpecularTexture : register(t1);
    elseif: BUMP
        Texture2D NormalTexture   : register(t1);
    endif
endif

struct Attributes
{
    float4 position               : SV_POSITION;
    float2 uvs                    : TEXCOORD0;
    ifdef: !FLAT               
        float3 normal             : NORMAL;
        float3 vertToLight        : TEXCOORD1;
        ifdef: SPECULAR       
            float3 vertToCamera   : TEXCOORD2;
        endif
    endif
};

Attributes VShader(float4 position    : POSITION,    
                   float2 uvs         : TEXCOORD0,
                   float3 normal      : NORMAL)
{
    Attributes output;

    output.position = mul(viewProjection, position);
    output.uvs = uvs;
    
    ifdef: !FLAT
        output.normal = normal;
        output.vertToLight = normalize(lightPosition - position.xyz);
        ifdef: SPECULAR
            output.vertToCamera = normalize(cameraPosition - position.xyz);
        endif
    endif

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColour = DiffuseTexture.Sample(Sampler, input.uvs);
    
    ifdef: !FLAT
        float3 normal = normalize(input.normal);

        ifdef: BUMP
            float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
        endif
        
        finalColour.rgb *= lightDiffuse * ((dot(input.vertToLight, normal) + 1.0) * 0.5);
                
        ifdef: SPECULAR
            float specularity = lightSpecularity * meshSpecularity;
            float4 specularTex = SpecularTexture.Sample(Sampler, input.uvs);
            float3 halfVector = normalize(input.vertToLight + input.vertToCamera);
            float specular = pow(max(dot(normal, halfVector), 0.0), specularity); 
            finalColour.rgb += specular * specularTex.rgb * lightSpecular;
        endif
    endif

    return finalColour * meshAmbience;
}