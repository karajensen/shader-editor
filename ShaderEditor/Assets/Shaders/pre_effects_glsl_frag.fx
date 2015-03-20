////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[EFFECTS_TEXTURES];

uniform float normalMask;
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

    // Create the Bloom
    // map range from end->start to 0->1
    vec3 bloom = vec3(bloomStart, bloomStart - bloomFade, 1.0);
    bloom = (scene.rgb-bloom.ggg)*(bloom.bbb/(bloom.rrr-bloom.ggg));
    bloom = clamp(bloom, 0.0, 1.0);
    out_Color[ID_COLOUR].a = max(bloom.r, max(bloom.b, bloom.g));

    // Ambient Occlusion
    vec4 normals = texelFetch(NormalSampler, uvs, 0);
    vec4 random = texture(RandomSampler, ex_UVs * vec2(RANDOM_UVS));

    out_Color[ID_EFFECTS].rgb = random.rgb * (1.0 - normalMask);
    out_Color[ID_EFFECTS].rgb += normals.rgb * normalMask;
    out_Color[ID_EFFECTS].a = normals.a;
}