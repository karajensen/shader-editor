////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_depth_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
in float ex_Depth;
out vec4 out_Color;
 
void main(void)
{
    out_Color = vec4(0.0, 1.0, 0.0, ex_Depth);
}