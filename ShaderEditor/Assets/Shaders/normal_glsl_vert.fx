////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec3 ex_Normal;
out float ex_Depth;

uniform mat4 viewProjection;
uniform vec2 frustum;
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    ex_Normal = in_Normal;

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z-frustum.x)*
        ((depthBounds.x-depthBounds.y)/(frustum.y-frustum.x)))+depthBounds.y;
}