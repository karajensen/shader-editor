#version 150

in vec3 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform float testing;
uniform mat4 projMat;
uniform mat4 viewMat;
 
void main(void)
{
	ex_Color = in_Color;
	ex_Color.g *= testing;
	gl_Position = projMat * viewMat * vec4(in_Position, 1.0);
}