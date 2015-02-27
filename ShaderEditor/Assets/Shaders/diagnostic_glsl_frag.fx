////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - diagnostic_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec3 ex_PositionWorld;
in vec3 ex_Normal;

out vec4 out_Color;

uniform vec3 meshColour;

void main(void)
{
    vec3 vertToLight = vec3(DIAGNOSTIC_LIGHT) - ex_PositionWorld;
    float diffuse = ((dot(normalize(vertToLight), normalize(ex_Normal))+1.0)*0.5);
    out_Color.rgb = meshColour * diffuse;
    out_Color.a = 0.0;
}