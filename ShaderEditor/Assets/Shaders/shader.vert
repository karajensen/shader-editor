#version 150

in vec3 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec3 ex_VertToLight;
out vec3 ex_Normal;

uniform mat4 viewProjection;
uniform vec3 lightPosition;
 
void main(void)
{
	gl_Position = viewProjection * vec4(in_Position, 1.0);
	gl_TexCoord[0].st = in_UVs;
	ex_Normal = in_Normal;
	ex_VertToLight = lightPosition - in_Position;
}