
#version 150

out vec4 out_Color[2];

in float ex_Depth;
in vec2 ex_UVs;

uniform float meshAmbience;

uniform sampler2D DiffuseSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    out_Color[0].rgb = diffuseTex.rgb;
    out_Color[0].rgb *= meshAmbience;
    out_Color[0].a = 1.0;
    out_Color[1] = vec4(ex_Depth, ex_Depth, ex_Depth, 1.0);
}
