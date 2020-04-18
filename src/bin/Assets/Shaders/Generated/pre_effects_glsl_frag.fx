
#version 150

in vec2 ex_UVs;

out vec4 out_Color;

uniform float bloomStart;
uniform float bloomFade;

uniform sampler2DMS SceneSampler;

void main(void)
{
    
    ivec2 uvs = ivec2(ex_UVs.x * 800, ex_UVs.y * 600);
    out_Color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; ++i)
    {
        out_Color += texelFetch(SceneSampler, uvs, i);
    }
    out_Color *= (1.0 / 4);

    vec3 bloom = vec3(bloomStart, bloomStart - bloomFade, 1.0);
    bloom = (out_Color.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    bloom = clamp(bloom, 0.0, 1.0);
    out_Color.a = max(bloom.r, max(bloom.b, bloom.g));
}
