#version 150

in vec3 ex_VertToLight;
in vec3 ex_Normal;

out vec4 out_Color;
 
void main(void)
{
    vec4 finalColour;
    normalize(ex_Normal);

    float diffuse = (dot(ex_VertToLight, ex_Normal) + 1.0) * 0.5;
    
    finalColour.rgb = vec3(diffuse);
    finalColour.a = 1.0;
    out_Color = finalColour;
}