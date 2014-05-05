#version 150

in vec3 ex_VertToLight;
in vec3 ex_Normal;

out vec4 out_Color;
 
void main(void)
{
    vec4 finalColour;
	vec3 normal = ex_Normal;
    normalize(normal);

    float diffuse = (dot(ex_VertToLight, normal) + 1.0) * 0.5; 
    
    finalColour.r = diffuse;
	finalColour.g = diffuse;
	finalColour.b = diffuse;
    finalColour.a = 1.0;
    out_Color = finalColour;
}