////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadow_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in vec2 ex_UVs;
 
uniform sampler2D DiffuseSampler;

void main(void)
{
	out_Color[ID_COLOUR] = texture(DiffuseSampler, ex_UVs);
	out_Color[ID_COLOUR].a = 1.0;
	out_Color[ID_DEPTH] = vec4(1.0, 1.0, 1.0, 1.0);
}