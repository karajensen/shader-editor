#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec3 ex_VertToLight;
out vec3 ex_Normal;

uniform mat4 viewProjection;
uniform vec3 lightPosition;
 
void main(void)
{
	gl_Position = viewProjection * in_Position;
    gl_TexCoord[0].st = in_UVs;
	ex_Normal = in_Normal;
	ex_VertToLight = normalize(lightPosition - in_Position.xyz);
}