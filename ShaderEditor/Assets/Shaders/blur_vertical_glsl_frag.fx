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

vec4 Blur(sampler2D samplerName, vec2 uvs)
{
    vec4 uvSteps = vec4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    vec2 uvs1p = vec2(uvs.x, uvs.y + uvSteps.x);
    vec2 uvs1n = vec2(uvs.x, uvs.y - uvSteps.x);
    vec2 uvs2p = vec2(uvs.x, uvs.y + uvSteps.y);
    vec2 uvs2n = vec2(uvs.x, uvs.y - uvSteps.y);
    vec2 uvs3p = vec2(uvs.x, uvs.y + uvSteps.z);
    vec2 uvs3n = vec2(uvs.x, uvs.y - uvSteps.z);
    vec2 uvs4p = vec2(uvs.x, uvs.y + uvSteps.w);
    vec2 uvs4n = vec2(uvs.x, uvs.y - uvSteps.w);

    vec4 colour = texture(samplerName, uvs) * weightMain;
    colour += texture(samplerName, uvs1p) * weightOffset.x;
    colour += texture(samplerName, uvs1n) * weightOffset.x;
    colour += texture(samplerName, uvs2p) * weightOffset.y;
    colour += texture(samplerName, uvs2n) * weightOffset.y;
    colour += texture(samplerName, uvs3p) * weightOffset.z;
    colour += texture(samplerName, uvs3n) * weightOffset.z;
    colour += texture(samplerName, uvs4p) * weightOffset.w;
    colour += texture(samplerName, uvs4n) * weightOffset.w;
    return colour;
}

void main(void)
{
    out_Color[ID_BLUR_SCENE] = Blur(SceneSampler, ex_UVs);
    out_Color[ID_BLUR_EFFECTS] = Blur(EffectsSampler, ex_UVs);
}