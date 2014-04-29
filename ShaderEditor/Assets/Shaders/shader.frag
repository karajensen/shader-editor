#version 150

in vec4 ex_Color;
in vec3 ex_Normal;
out vec4 out_Color;
 
void main(void)
{
	out_Color = ex_Color;
	out_Color.r = ex_Normal.g;
}