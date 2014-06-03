#version 150

in vec4 ex_Position;
out vec4 out_Color;
 
void main(void)
{
    out_Color = ex_Position;
}