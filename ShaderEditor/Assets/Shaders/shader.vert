#version 150

in vec3 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec4 ex_Color;
out vec3 ex_Normal;

uniform float testing;
uniform mat4 projMat;
uniform mat4 viewMat;
 
void main(void)
{
	gl_Position = projMat * viewMat * vec4(in_Position, 1.0);
	gl_TexCoord[0].st = in_UVs;
	ex_Normal = in_Normal;
	ex_Color = vec4(testing, testing, testing, 1.0);
}