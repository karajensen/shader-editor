////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
ifndefined: FLAT
    in vec3 ex_VertToLight;
    in vec3 ex_Normal;
    ifdefined: SPECULAR
        in vec3 ex_VertToCamera;
    endif
endif

uniform float meshAmbience;
ifndefined: FLAT
    uniform vec3 lightDiffuse;
    uniform vec3 lightAttenuation;
    ifdefined: SPECULAR
        uniform vec3 lightSpecular;
        uniform float lightSpecularity;
        uniform float meshSpecularity;
    endif
endif

uniform sampler2D DiffuseSampler;
ifndefined: FLAT
    ifdefined: SPECULAR
        uniform sampler2D SpecularSampler;
    endif
    ifdefined: BUMP
        uniform sampler2D NormalSampler;
    endif
endif
 
void main(void)
{
    vec4 finalColour = texture(DiffuseSampler, ex_UVs);
    
    ifndefined: FLAT
        normalize(ex_Normal);

        ifdefined: BUMP
            vec4 normalTex = texture(NormalSampler, ex_UVs);
            finalColour.a = normalTex.a;
        endif       
        
        finalColour.rgb *= (dot(ex_VertToLight, ex_Normal) + 1.0) * 0.5;
                
        ifdefined: SPECULAR
            // Specular Blinn-Phong
            float specularity = 5.0 * meshSpecular;
            vec4 specularTex = texture(SpecularSampler, ex_UVs);
            finalColour.a = specularTex.a;
        endif
    endif   

    out_Color = finalColour * meshAmbience;
}