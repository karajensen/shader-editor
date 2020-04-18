
#version 150

out vec4 out_Color[2];

in vec2 ex_UVs;
 
uniform sampler2D DiffuseSampler;

void main(void)
{
	out_Color[0] = texture(DiffuseSampler, ex_UVs);
	out_Color[0].a = 1.0;
	out_Color[1] = vec4(1.0, 1.0, 1.0, 1.0);
}
