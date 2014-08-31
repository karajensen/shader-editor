////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
in vec3 ex_Normal;
in float ex_Depth;
ifdef: BUMP
    in vec3 ex_Tangent;
    in vec3 ex_Bitangent;
endif

ifdef: BUMP
    uniform sampler2D NormalSampler;
    uniform float meshBump;
endif

out vec4 out_Color;
 
void main(void)
{
    vec3 normal = normalize(ex_Normal);
    ifdef: BUMP
        vec4 normalTex = texture(NormalSampler, ex_UVs);
        vec2 bump = meshBump * (normalTex.rg - 0.5);
        normal = normalize(normal + bump.x * normalize(ex_Tangent) + bump.y * normalize(ex_Bitangent));
    endif

    out_Color = vec4(normal, ex_Depth);
}