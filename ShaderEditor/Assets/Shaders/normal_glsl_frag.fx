#version 150

in vec3 ex_Normal;
in float ex_Depth;

out vec4 out_Color;
 
void main(void)
{
    out_Color = vec4(normalize(ex_Normal), ex_Depth);
}