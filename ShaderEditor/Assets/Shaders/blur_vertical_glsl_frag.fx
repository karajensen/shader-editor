////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_vertical_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color[BLUR_TEXTURES];

uniform sampler2D SceneSampler;
uniform sampler2D EffectsSampler;

uniform float blurStep;

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

    out_Color[ID_BLUR_SCENE] =  texture(SceneSampler, ex_UVs) * WEIGHT0;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs1p) * WEIGHT1;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs1n) * WEIGHT1;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs2p) * WEIGHT2;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs2n) * WEIGHT2;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs3p) * WEIGHT3;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs3n) * WEIGHT3;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs4p) * WEIGHT4;
    out_Color[ID_BLUR_SCENE] += texture(SceneSampler, uvs4n) * WEIGHT4;

    out_Color[ID_BLUR_EFFECTS] =  texture(EffectsSampler, ex_UVs) * WEIGHT0;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs1p) * WEIGHT1;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs1n) * WEIGHT1;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs2p) * WEIGHT2;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs2n) * WEIGHT2;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs3p) * WEIGHT3;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs3n) * WEIGHT3;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs4p) * WEIGHT4;
    out_Color[ID_BLUR_EFFECTS] += texture(EffectsSampler, uvs4n) * WEIGHT4;
}