////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2D SceneSampler;

uniform float horizontalPass;
uniform float verticalPass;
uniform float blurAmount;
uniform float blurStep;
uniform float weightMain;
uniform vec4 weightOffset;

void main(void)
{
    vec4 uvSteps = vec4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    vec4 horizontal = uvSteps * horizontalPass;
    vec4 vertical = uvSteps * verticalPass;
    float blurring = max(1.0, blurAmount * verticalPass);

    vec2 uvs1p = vec2(ex_UVs.x + horizontal.x, ex_UVs.y + vertical.x);
    vec2 uvs1n = vec2(ex_UVs.x - horizontal.x, ex_UVs.y - vertical.x);
    vec2 uvs2p = vec2(ex_UVs.x + horizontal.y, ex_UVs.y + vertical.y);
    vec2 uvs2n = vec2(ex_UVs.x - horizontal.y, ex_UVs.y - vertical.y);
    vec2 uvs3p = vec2(ex_UVs.x + horizontal.z, ex_UVs.y + vertical.z);
    vec2 uvs3n = vec2(ex_UVs.x - horizontal.z, ex_UVs.y - vertical.z);
    vec2 uvs4p = vec2(ex_UVs.x + horizontal.w, ex_UVs.y + vertical.w);
    vec2 uvs4n = vec2(ex_UVs.x - horizontal.w, ex_UVs.y - vertical.w);

    out_Color = texture(SceneSampler, ex_UVs) * weightMain;
    out_Color += texture(SceneSampler, uvs1p) * weightOffset.x;
    out_Color += texture(SceneSampler, uvs1n) * weightOffset.x;
    out_Color += texture(SceneSampler, uvs2p) * weightOffset.y;
    out_Color += texture(SceneSampler, uvs2n) * weightOffset.y;
    out_Color += texture(SceneSampler, uvs3p) * weightOffset.z;
    out_Color += texture(SceneSampler, uvs3n) * weightOffset.z;
    out_Color += texture(SceneSampler, uvs4p) * weightOffset.w;
    out_Color += texture(SceneSampler, uvs4n) * weightOffset.w;
    out_Color *= blurring;
}