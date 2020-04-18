
#version 150

in vec3 ex_PositionWorld;
in vec3 ex_Normal;

out vec4 out_Color;

void main(void)
{
    vec3 vertToLight = vec3(10.0, 10.0, 0.0) - ex_PositionWorld;
    float diffuse = ((dot(normalize(vertToLight), normalize(ex_Normal))+1.0)*0.5);
    out_Color.r = diffuse;
    out_Color.g = diffuse;
    out_Color.b = diffuse;
    out_Color.a = 0.0;
}
