////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_normal_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec2 ex_UVs;
out vec3 ex_Normal;
out float ex_Depth;

uniform mat4 viewProjection;
uniform float depthNear;
uniform float depthFar;
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    ex_UVs = in_UVs;
    ex_Normal = in_Normal;

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
}