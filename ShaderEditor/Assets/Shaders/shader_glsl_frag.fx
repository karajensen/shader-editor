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
    ifdef: SPECULAR
        uniform sampler2D SpecularSampler;
    endif
    ifdef: BUMP
        uniform sampler2D NormalSampler;
    endif
endif
 
void main(void)
{
    vec4 finalColour = texture(DiffuseSampler, ex_UVs);
    
    ifdef: !FLAT
        vec3 normal = normalize(ex_Normal);

        ifdef: BUMP
            vec4 normalTex = texture(NormalSampler, ex_UVs);
            finalColour.a = normalTex.a;
        endif       
        
        finalColour.rgb *= lightDiffuse * ((dot(ex_VertToLight, normal) + 1.0) * 0.5);
                
        ifdef: SPECULAR
            float specularity = lightSpecularity * meshSpecularity;
            vec4 specularTex = texture(SpecularSampler, ex_UVs);
            vec3 halfVector = normalize(ex_VertToLight + ex_VertToCamera);
            float specular = pow(max(dot(normal, halfVector), 0.0), specularity); 
            finalColour.rgb += specular * specularTex.rgb * lightSpecular;
        endif
    endif   

    out_Color = finalColour * meshAmbience;
}