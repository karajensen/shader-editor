
#version 150

out vec4 out_Color[2];

in float ex_Depth;
in vec2 ex_UVs;
 
uniform sampler2D DiffuseSampler;
uniform float alpha;
uniform vec4 tint;

void main(void)
{
    out_Color[0] = texture(DiffuseSampler, ex_UVs) * tint;
    out_Color[0].a *= alpha;
    out_Color[1] = vec4(ex_Depth, ex_Depth, ex_Depth, out_Color[0].a);
}
