////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;
 
uniform sampler2D DiffuseSampler;
uniform float alpha;
uniform vec4 tint;

void main(void)
{
    out_Color.rgb = texture(DiffuseSampler, ex_UVs).rgb * tint.rgb;
    out_Color.a = tint.a * alpha;
}