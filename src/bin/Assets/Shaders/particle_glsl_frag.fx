////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in float ex_Depth;
in vec2 ex_UVs;
 
uniform sampler2D DiffuseSampler;
uniform float alpha;
uniform vec4 tint;

void main(void)
{
    out_Color[ID_COLOUR] = texture(DiffuseSampler, ex_UVs) * tint;
    out_Color[ID_COLOUR].a *= alpha;
    out_Color[ID_DEPTH] = vec4(ex_Depth, ex_Depth, ex_Depth, out_Color[ID_COLOUR].a);
}