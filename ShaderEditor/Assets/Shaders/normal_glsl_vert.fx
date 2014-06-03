#version 150

in vec4 in_Position;
in vec2 in_UVs;
out vec4 ex_Position;
 
void main(void)
{
    gl_Position = in_Position;
    gl_TexCoord[0].st = in_UVs;
    ex_Position = in_Position;
}