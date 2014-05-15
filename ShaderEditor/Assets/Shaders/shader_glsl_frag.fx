#version 150

in vec3 ex_VertToLight;
in vec3 ex_Normal;

out vec4 out_Color;

uniform sampler2D DiffuseSampler;
uniform sampler2D SpecularSampler;
uniform sampler2D NormalSampler;
 
void main(void)
{
    vec4 finalColour;
    normalize(ex_Normal);

    float diffuse = (dot(ex_VertToLight, ex_Normal) + 1.0) * 0.5;
    
    vec4 diffuseTex = texture(DiffuseSampler, gl_TexCoord[0].st);
    vec4 specularTex = texture(SpecularSampler, gl_TexCoord[0].st);
    vec4 normalTex = texture(NormalSampler, gl_TexCoord[0].st);

    finalColour = specularTex;
    finalColour.rgb *= vec3(diffuse);
    finalColour.a = diffuseTex.a + normalTex.a;
    out_Color = finalColour;
}