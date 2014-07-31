cbuffer VertexBuffer : register(b0)
{
    float4x4 viewProjection;
    ifndefined: FLAT
        float3 lightPosition;
        ifdefined: SPECULAR
            float3 cameraPosition;
        endif
    endif
};

SamplerState Sampler;
Texture2D DiffuseTexture          : register(t0);
ifndefined: FLAT
    ifdefined: SPECULAR|BUMP
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
    ifndefined: FLAT               
        float3 normal             : NORMAL;
        float3 vertToLight        : TEXCOORD1;
        ifdefined: SPECULAR       
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
    
    ifndefined: FLAT
        output.normal = normal;
        output.vertToLight = normalize(lightPosition - position.xyz);
        ifdefined: SPECULAR
            output.vertToCamera = normalize(cameraPosition - position.xyz);
        endif
    endif

    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColour = DiffuseTexture.Sample(Sampler, input.uvs);
    
    ifndefined: FLAT
        normalize(input.normal);

        ifdefined: BUMP
            float4 normalTex = NormalTexture.Sample(Sampler, input.uvs);
        endif       
        
        finalColour.rgb *= (dot(input.vertToLight, input.normal) + 1.0) * 0.5;
                
        ifdefined: SPECULAR
            // Specular Blinn-Phong
            float specularity = 5.0;
            float4 specularTex = SpecularTexture.Sample(Sampler, input.uvs);
            float3 halfVector = normalize(input.vertToLight + input.vertToCamera);
            finalColour.rgb += pow(saturate(dot(input.normal, halfVector)), specularity);
        endif
    endif

    return finalColour;
}