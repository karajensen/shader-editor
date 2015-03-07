////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[EFFECTS_TEXTURES];

uniform float bloomIntensity;
uniform float bloomStart;
uniform float bloomFade;

uniform sampler2DMS SceneSampler;
uniform sampler2DMS NormalSampler;

vec4 GetMultisampled(sampler2DMS samplerName, ivec2 uvs)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += texelFetch(samplerName, uvs, i);
    }
    return  color * (1.0 / SAMPLES);
}

void main(void)
{
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    vec4 scene = GetMultisampled(SceneSampler, uvs);
    vec4 normal = GetMultisampled(NormalSampler, uvs);

    out_Color[ID_COLOUR] = scene;
    out_Color[ID_NORMAL] = normal;

    // Create the Bloom
    // map range from end->start to 0->1
    vec3 bloom = vec3(bloomStart, bloomStart - bloomFade, 1.0);
    out_Color[ID_EFFECTS].rgb = (scene.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    out_Color[ID_EFFECTS].rgb = clamp(out_Color[ID_EFFECTS].rgb, 0.0, 1.0);
    out_Color[ID_EFFECTS].rgb *= bloomIntensity;
}