////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer SceneVertexBuffer : register(b0)
{
    float4x4 viewProjection;
    float depthNear;
    float depthFar;
    ifdef: SPECULAR
        float3 cameraPosition;  
    endif
};

cbuffer MeshVertexBuffer : register(b1)
{
    float4x4 world;
};

cbuffer MeshPixelBuffer : register(b2)
{
    float meshAmbience;
    ifdef: !FLAT
        float meshDiffuse;
    endif
    ifdef: BUMP
        float meshBump;
    endif
    ifdef: SPECULAR
        float meshSpecular;
        float meshSpecularity;
    endif
    ifdef: CAUSTICS
        float meshCausticAmount;
        float meshCausticScale;
    endif
};

#ifdef: !FLAT
cbuffer ScenePixelBuffer : register(b3)
{
    float lightActive[MAX_LIGHTS];
    float3 lightPosition[MAX_LIGHTS];
    float3 lightAttenuation[MAX_LIGHTS];
    float3 lightDiffuse[MAX_LIGHTS];
    ifdef: SPECULAR
        float lightSpecularity[MAX_LIGHTS];
        float3 lightSpecular[MAX_LIGHTS];
    endif
};
endif

SamplerState Sampler;
Texture2D DiffuseSampler;
ifdef: BUMP
    Texture2D NormalSampler;
endif
ifdef: SPECULAR
    Texture2D SpecularSampler;
endif
ifdef: CAUSTICS
    Texture2D CausticsSampler;
endif

struct Attributes
{
    float4 position          : SV_POSITION;
    float3 normal            : NORMAL;
    float  depth             : TEXCOORD0;
    float2 uvs               : TEXCOORD1;
    ifdef: !FLAT
        float3 positionWorld : TEXCOORD2;
    endif
    ifdef: BUMP
        float3 tangent       : TEXCOORD3;
        float3 bitangent     : TEXCOORD4;
    endif
    ifdef: SPECULAR|BUMP          
        float3 vertToCamera  : TEXCOORD5;
    elseif: SPECULAR
        float3 vertToCamera  : TEXCOORD3;
    endif
};

struct Outputs
{
    float4 colour : SV_TARGET0;
    float4 depth  : SV_TARGET1;
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
    output.normal = mul(world, normal);
    output.uvs = uvs;
    ifdef: !FLAT
        output.positionWorld = mul(world, position).xyz;
    endif

    float2 depthBounds = float2(0.0, 1.0);
    output.depth = ((output.position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
    
    ifdef: BUMP
        output.tangent = mul(world, tangent);
        output.bitangent = mul(world, bitangent);
    endif

    ifdef: SPECULAR
        output.vertToCamera = cameraPosition - output.positionWorld;
    endif

    return output;
}

Outputs PShader(Attributes input)
{
    float4 diffuseTex = DiffuseSampler.Sample(Sampler, input.uvs);

    ifdef: !FLAT
        float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
        float3 normal = normalize(input.normal);
    endif

    ifdef: BUMP
        float4 normalTex = NormalSampler.Sample(Sampler, input.uvs);
        float2 bump = meshBump * (normalTex.rg - 0.5);
        normal = normalize(normal + bump.x * 
            normalize(input.tangent) + bump.y * normalize(input.bitangent));
    endif

    ifdef: SPECULAR
        float3 vertToCamera = normalize(input.vertToCamera);
        float4 specularTex = SpecularSampler.Sample(Sampler, input.uvs);
        float4 specular = float4(0.0, 0.0, 0.0, 0.0);
    endif

    ifdef: !FLAT
        for (int i = 0; i < MAX_LIGHTS; ++i)
        {
            float3 lightColour = lightDiffuse[i];
            float3 vertToLight = lightPosition[i] - input.positionWorld;
            float lightLength = length(vertToLight);
        
            float attenuation = 1.0 / (lightAttenuation[i].x 
                + lightAttenuation[i].y * lightLength 
                + lightAttenuation[i].z * lightLength * lightLength);

            vertToLight /= lightLength;

            lightColour *= ((dot(vertToLight, normal) + 1.0) *
                ((1.0 - meshDiffuse) * 0.5)) + meshDiffuse;

            diffuse.rgb += lightColour * attenuation * lightActive[i];

            ifdef: SPECULAR
                float specularity = lightSpecularity[i] * meshSpecularity;
                float3 halfVector = normalize(vertToLight + vertToCamera);
                float specularFactor = pow(max(dot(normal, halfVector), 0.0), specularity); 
                specular.rgb += specularFactor * lightSpecular[i] * 
                    attenuation * lightActive[i] * meshSpecular;
            endif
        }
    endif

    ifdef: CAUSTICS
        float3 caustics = CausticsSampler.Sample(
            Sampler, input.uvs * meshCausticScale).rgb * max(normal.y, 0.0);
    endif

    Outputs output;
    output.depth = float4(input.depth, input.depth, input.depth, 1.0);

    ifdef: FLAT
        output.colour.rgb = diffuseTex.rgb;
    else:
        output.colour.rgb = diffuseTex.rgb * diffuse;
    endif
    ifdef: SPECULAR
        output.colour.rgb += specularTex.rgb * specular;
    endif
    ifdef: CAUSTICS
        output.colour.rgb += caustics * meshCausticAmount;
    endif
    output.colour.rgb *= meshAmbience;
    output.colour.a = 1.0;

    return output;
}