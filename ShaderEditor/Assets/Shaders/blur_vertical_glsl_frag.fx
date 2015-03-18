////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_vertical_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[BLUR_TEXTURES];

uniform sampler2D SceneSampler;
uniform sampler2D EffectsSampler;

uniform float blurStep;
uniform float weightMain;
uniform vec4 weightOffset;

void main(void)
{
    vec4 uvSteps = vec4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    vec2 uvs1p = vec2(ex_UVs.x, ex_UVs.y + uvSteps.x);
    vec2 uvs1n = vec2(ex_UVs.x, ex_UVs.y - uvSteps.x);
    vec2 uvs2p = vec2(ex_UVs.x, ex_UVs.y + uvSteps.y);
    vec2 uvs2n = vec2(ex_UVs.x, ex_UVs.y - uvSteps.y);
    vec2 uvs3p = vec2(ex_UVs.x, ex_UVs.y + uvSteps.z);
    vec2 uvs3n = vec2(ex_UVs.x, ex_UVs.y - uvSteps.z);
    vec2 uvs4p = vec2(ex_UVs.x, ex_UVs.y + uvSteps.w);
    vec2 uvs4n = vec2(ex_UVs.x, ex_UVs.y - uvSteps.w);

    out_Color[ID_BLUR_SCENE] =  texture(SceneSampler, ex_UVs) * weightMain;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs1p) * weightOffset.x;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs1n) * weightOffset.x;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs2p) * weightOffset.y;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs2n) * weightOffset.y;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs3p) * weightOffset.z;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs3n) * weightOffset.z;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs4p) * weightOffset.w;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs4n) * weightOffset.w;

    out_Color[ID_BLUR_EFFECTS] =  texture(EffectsSampler, ex_UVs) * weightMain;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs1p) * weightOffset.x;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs1n) * weightOffset.x;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs2p) * weightOffset.y;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs2n) * weightOffset.y;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs3p) * weightOffset.z;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs3n) * weightOffset.z;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs4p) * weightOffset.w;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs4n) * weightOffset.w;
}