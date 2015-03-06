////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - pre_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[EFFECTS_TEXTURES];

uniform float bloomIntensity;
uniform float bloomStart;

uniform sampler2DMS SceneSampler;
uniform sampler2DMS NormalSampler;

vec3 saturate(vec3 value)
{
    return vec3(min(1.0, max(value.r, 0.0)),
                min(1.0, max(value.g, 0.0)),
                min(1.0, max(value.b, 0.0)));
}

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
    vec4 scene = GetMultisampled(SceneSampler, uvs);
    vec4 normal = GetMultisampled(NormalSampler, uvs);

    out_Color[ID_COLOUR] = scene;
    out_Color[ID_NORMAL] = normal;

    vec3 outer = vec3(1.0, 1.0, 1.0);
    vec3 inner = vec3(bloomStart, bloomStart, bloomStart);
    out_Color[ID_EFFECTS].rgb = saturate((scene.rgb-inner)*(outer/(outer-inner)));
    out_Color[ID_EFFECTS].rgb *= bloomIntensity;
}