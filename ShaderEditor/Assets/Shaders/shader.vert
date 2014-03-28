#version 130

in vec4 in_Position;
in vec3 in_Color;
out vec3 ex_Color;
uniform mat4 viewProjection;
 
void main(void)
{
	ex_Color = in_Color;
	gl_Position = viewProjection * in_Position;
}