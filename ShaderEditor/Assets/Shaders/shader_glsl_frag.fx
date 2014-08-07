////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
ifdef: !FLAT
    in vec3 ex_VertToLight;
    in vec3 ex_Normal;
    ifdef: SPECULAR
        in vec3 ex_VertToCamera;
    endif
endif

uniform float meshAmbience;
ifdef: !FLAT
    uniform vec3 lightDiffuse;
    uniform vec3 lightAttenuation;
    ifdef: SPECULAR
        uniform vec3 lightSpecular;
        uniform float lightSpecularity;
        uniform float meshSpecularity;
    endif
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
    
    ifdef: !FLAT
        float lightLen = length(ex_VertToLight);
        float attenuation = 1.0 / (lightAttenuation.x 
            + lightAttenuation.y * lightLen 
            + lightAttenuation.z * lightLen * lightLen);

        vec3 normal = normalize(ex_Normal);
        vec3 vertToLight = ex_VertToLight / lightLen;

        ifdef: BUMP
            vec4 normalTex = texture(NormalSampler, ex_UVs);
            out_Color.a = normalTex.a;
        endif       
        
        out_Color.rgb *= lightDiffuse * ((dot(vertToLight, normal) + 1.0) * 0.5);
                
        ifdef: SPECULAR
            float specularity = lightSpecularity * meshSpecularity;
            vec4 specularTex = texture(SpecularSampler, ex_UVs);
            vec3 halfVector = normalize(vertToLight + normalize(ex_VertToCamera));
            float specular = pow(max(dot(normal, halfVector), 0.0), specularity); 
            out_Color.rgb += specular * specularTex.rgb * lightSpecular;
        endif
    endif   

    // TEMP FOR FIXING MULTIPLE TEXTURES
    ifdef: SPECULAR|BUMP
    out_Color = specularTex;
    out_Color.a = normalTex.a;
    else:

    out_Color *= meshAmbience;
    ifdef: !FLAT
        out_Color *= attenuation;
    endif

    endif
}