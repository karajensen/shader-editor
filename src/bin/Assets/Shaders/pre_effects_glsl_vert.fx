////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
out vec2 ex_UVs;
 
void main(void)
{
    gl_Position = in_Position;
    ex_UVs = in_UVs;
}