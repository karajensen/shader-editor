#version 150

ifndefined: FLAT
    in vec3 ex_VertToLight;
    in vec3 ex_Normal;
endif
out vec4 out_Color;

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
    vec4 finalColour = texture(DiffuseSampler, gl_TexCoord[0].st);
    
    ifndefined: FLAT
        normalize(ex_Normal);

        ifdefined: BUMP
            vec4 normalTex = texture(NormalSampler, gl_TexCoord[0].st);
            finalColour.a = normalTex.a;
        endif       
        
        finalColour.rgb *= (dot(ex_VertToLight, ex_Normal) + 1.0) * 0.5;
                
        ifdefined: SPECULAR
            // Specular Blinn-Phong
            float specularity = 5.0;
            vec4 specularTex = texture(SpecularSampler, gl_TexCoord[0].st);
            finalColour.a = specularTex.a;
        endif
    endif   

    //finalColour.a = 1.0;
    out_Color = finalColour;
}