#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_UVs;
out vec3 ex_Color;
uniform mat4 viewProjection;
uniform float testing;
 
void main(void)
{
	ex_Color = in_Color;
	ex_Color.r *= viewProjection[0][0];
	ex_Color.g = testing;
	ex_Color.b = in_UVs.x;

	gl_Position = viewProjection * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position.xy, 0.0, 1.0);
}