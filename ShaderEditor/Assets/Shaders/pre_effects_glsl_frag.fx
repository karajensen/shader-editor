////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[EFFECTS_TEXTURES];

uniform sampler2DMS SceneSampler;
uniform sampler2DMS NormalSampler;

vec4 GetMultisampled(sampler2DMS samplerName, ivec2 uvs)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += texelFetch(samplerName, uvs, i);
    }
    return color * (1.0 / SAMPLES);
}

void main(void)
{
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    out_Color[ID_COLOUR] = GetMultisampled(SceneSampler, uvs);
    out_Color[ID_NORMAL] = GetMultisampled(NormalSampler, uvs);
    out_Color[ID_EFFECTS] = vec4(1.0, 1.0, 0.0, 1.0);
}