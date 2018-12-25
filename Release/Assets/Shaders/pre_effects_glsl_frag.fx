////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color;

uniform float bloomStart;
uniform float bloomFade;

uniform sampler2DMS SceneSampler;

void main(void)
{
    // Multisample the scene
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    out_Color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        out_Color += texelFetch(SceneSampler, uvs, i);
    }
    out_Color *= (1.0 / SAMPLES);

    // Create the Bloom
    // map range from end->start to 0->1
    vec3 bloom = vec3(bloomStart, bloomStart - bloomFade, 1.0);
    bloom = (out_Color.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    bloom = clamp(bloom, 0.0, 1.0);
    out_Color.a = max(bloom.r, max(bloom.b, bloom.g));
}