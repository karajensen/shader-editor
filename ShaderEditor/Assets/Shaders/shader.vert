#version 130

in vec4 in_Position;
in vec3 in_Color;
out vec3 ex_Color;
uniform mat4 viewProjection;
uniform float testing;
 
void main(void)
{
	ex_Color = in_Color;
	ex_Color.r *= viewProjection[0][0];
	ex_Color.g = testing;

	gl_Position = in_Position;
	//gl_Position = viewProjection * in_Position;
}