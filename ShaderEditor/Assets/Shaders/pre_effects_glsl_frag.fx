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
uniform sampler2D RandomSampler;

void main(void)
{
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    vec4 scene = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        scene += texelFetch(SceneSampler, uvs, i);
    }
    scene *= (1.0 / SAMPLES);

    out_Color[ID_COLOUR] = scene;
    out_Color[ID_NORMAL] = texelFetch(NormalSampler, uvs, 0);

    // Create the Bloom
    // map range from end->start to 0->1
    vec3 bloom = vec3(bloomStart, bloomStart - bloomFade, 1.0);
    out_Color[ID_EFFECTS].rgb = (scene.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    out_Color[ID_EFFECTS].rgb = clamp(out_Color[ID_EFFECTS].rgb, 0.0, 1.0);
    out_Color[ID_EFFECTS].rgb *= bloomIntensity;

    // Ambient Occlusion
    vec4 random = texture(RandomSampler, ex_UVs * vec2(RANDOM_UVS));
    out_Color[ID_EFFECTS].a = 1.0;
}