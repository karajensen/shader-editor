////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
in vec3 ex_VertToLight;
ifdef: !FLAT
    in vec3 ex_Normal;
    ifdef: BUMP
        in vec3 ex_Tangent;
        in vec3 ex_Bitangent;
    endif
    ifdef: SPECULAR
        in vec3 ex_VertToCamera;
    endif
endif

uniform float meshAmbience;
uniform vec3 lightDiffuse;
uniform vec3 lightAttenuation;
ifdef: !FLAT|BUMP
    uniform float meshBump;
endif
ifdef: !FLAT|SPECULAR
    uniform vec3 lightSpecular;
    uniform float lightSpecularity;
    uniform float meshSpecularity;
endif

uniform sampler2D DiffuseSampler;
ifdef: !FLAT
    ifdef: SPECULAR|BUMP
        uniform sampler2D NormalSampler;
        uniform sampler2D SpecularSampler;
    elseif: SPECULAR
        uniform sampler2D SpecularSampler;
    elseif: BUMP
        uniform sampler2D NormalSampler;
    endif
endif
 
void main(void)
{
    out_Color = texture(DiffuseSampler, ex_UVs);
    out_Color.rgb *= lightDiffuse;

    float lightLen = length(ex_VertToLight);
    float attenuation = 1.0 / (lightAttenuation.x 
        + lightAttenuation.y * lightLen 
        + lightAttenuation.z * lightLen * lightLen);
    vec3 vertToLight = ex_VertToLight / lightLen;

    ifdef: !FLAT

        vec3 normal = normalize(ex_Normal);
        ifdef: BUMP
            vec4 normalTex = texture(NormalSampler, ex_UVs);
            vec2 bump = meshBump * (normalTex.rg - 0.5);
            normal = normalize(normal + bump.x * normalize(ex_Tangent) + bump.y * normalize(ex_Bitangent));
        endif

        out_Color.rgb *= ((dot(vertToLight, normal) + 1.0) * 0.5);
                
        ifdef: SPECULAR
            float specularity = lightSpecularity * meshSpecularity;
            vec4 specularTex = texture(SpecularSampler, ex_UVs);
            vec3 halfVector = normalize(vertToLight + normalize(ex_VertToCamera));
            float specular = pow(max(dot(normal, halfVector), 0.0), specularity); 
            out_Color.rgb += specular * specularTex.rgb * lightSpecular;
        endif

    endif   

    out_Color *= meshAmbience * attenuation;
}