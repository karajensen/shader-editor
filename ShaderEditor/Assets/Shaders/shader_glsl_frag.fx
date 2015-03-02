////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in float ex_Depth;
in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;
ifdef: BUMP
    in vec3 ex_Tangent;
    in vec3 ex_Bitangent;
endif
ifdef: SPECULAR
    in vec3 ex_VertToCamera;
endif

uniform float lightActive[MAX_LIGHTS];
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];
ifdef: SPECULAR
    uniform vec3 lightSpecular[MAX_LIGHTS];
    uniform float lightSpecularity[MAX_LIGHTS];
    uniform float meshSpecularity;
endif

uniform float meshAmbience;
ifdef: BUMP
    uniform float meshBump;
endif

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 normal = normalize(ex_Normal);

    ifdef: BUMP
        vec4 normalTex = texture(NormalSampler, ex_UVs);
        vec2 bump = meshBump * (normalTex.rg - 0.5);
        normal = normalize(normal + bump.x * normalize(ex_Tangent) 
            + bump.y * normalize(ex_Bitangent));
    endif

    ifdef: SPECULAR
        vec3 vertToCamera = normalize(ex_VertToCamera);
        vec4 specularTex = texture(SpecularSampler, ex_UVs);
        vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    endif

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec3 lightColour = lightDiffuse[i];
        vec3 vertToLight = lightPosition[i] - ex_PositionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        ifdef: !FLAT
            vertToLight /= lightLength;
            lightColour *= ((dot(vertToLight, normal) + 1.0) * 0.5);
        endif

        ifdef: SPECULAR
            float specularity = lightSpecularity[i] * meshSpecularity;
            vec3 halfVector = normalize(vertToLight + vertToCamera);
            float specularFactor = pow(max(dot(normal, halfVector), 0.0), specularity); 
            specular.rgb += specularFactor * lightSpecular[i] * attenuation * lightActive[i];
        endif
        
        diffuse.rgb += lightColour * attenuation * lightActive[i];
    }

    out_Color[ID_COLOUR] = diffuseTex * diffuse;
    ifdef: SPECULAR
        out_Color[ID_COLOUR] += specularTex * specular;
    endif
    out_Color[ID_COLOUR].rgb *= meshAmbience;
    out_Color[ID_COLOUR].a = 1.0;

    out_Color[ID_NORMAL].rgb = normal;
    out_Color[ID_NORMAL].a = ex_Depth;
}