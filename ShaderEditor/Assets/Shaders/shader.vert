#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec3 ex_VertToLight;
out vec3 ex_Normal;

uniform mat4 viewProjection;
uniform mat4 viewInvTranspose;
uniform vec3 lightPosition;
 
void main(void)
{
	gl_Position = viewProjection * in_Position;
	ex_Normal = (viewInvTranspose * vec4(in_Normal, 1.0)).xyz;
	gl_TexCoord[0].st = in_UVs;

	vec4 lightViewPos = viewInvTranspose * vec4(lightPosition, 1.0);
	ex_VertToLight = normalize((lightViewPos - gl_Position).xyz);
}