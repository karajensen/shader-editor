////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
in vec3 ex_Normal;
in float ex_Depth;

out vec4 out_Color;
 
void main(void)
{
    // Temporary use uvs or normals are not outputted properly
    out_Color = vec4(normalize(ex_Normal), ex_Depth * ex_UVs.x);
}