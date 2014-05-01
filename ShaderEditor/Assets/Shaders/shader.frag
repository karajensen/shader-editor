#version 150

in vec3 ex_Normal;
out vec4 out_Color;
 
void main(void)
{
	out_Color = vec4(ex_Normal,1.0);
}