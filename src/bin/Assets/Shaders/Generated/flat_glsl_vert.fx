
#version 150

in vec4 in_Position;
in vec2 in_UVs;

out float ex_Depth;
out vec2 ex_UVs;

uniform float depthNear;
uniform float depthFar;
uniform mat4 world;
uniform mat4 viewProjection;
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;

    ex_Depth = ((gl_Position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;
}
