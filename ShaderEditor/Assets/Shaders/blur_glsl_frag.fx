////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneSampler;

uniform float horizontalPass;
uniform float verticalPass;
uniform float blurAmount;
uniform float weightMain;
uniform vec4 weightOffset;

void main(void)
{
    vec4 blur = vec4(blurAmount, blurAmount * 2.0, blurAmount * 3.0, blurAmount * 4.0);
    vec4 blurHorizontal = blur * horizontalPass;
    vec4 blurVertical = blur * verticalPass;

    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    ivec2 uvs1p = ivec2(uvs.x + blurHorizontal.x, uvs.y + blurVertical.x);
    ivec2 uvs1n = ivec2(uvs.x - blurHorizontal.x, uvs.y - blurVertical.x);
    ivec2 uvs2p = ivec2(uvs.x + blurHorizontal.y, uvs.y + blurVertical.y);
    ivec2 uvs2n = ivec2(uvs.x - blurHorizontal.y, uvs.y - blurVertical.y);
    ivec2 uvs3p = ivec2(uvs.x + blurHorizontal.z, uvs.y + blurVertical.z);
    ivec2 uvs3n = ivec2(uvs.x - blurHorizontal.z, uvs.y - blurVertical.z);
    ivec2 uvs4p = ivec2(uvs.x + blurHorizontal.w, uvs.y + blurVertical.w);
    ivec2 uvs4n = ivec2(uvs.x - blurHorizontal.w, uvs.y - blurVertical.w);

    out_Color = texelFetch(SceneSampler, uvs, 0) * weightMain;
    out_Color += texelFetch(SceneSampler, uvs1p, 0) * weightOffset.x;
    out_Color += texelFetch(SceneSampler, uvs1n, 0) * weightOffset.x;
    out_Color += texelFetch(SceneSampler, uvs2p, 0) * weightOffset.y;
    out_Color += texelFetch(SceneSampler, uvs2n, 0) * weightOffset.y;
    out_Color += texelFetch(SceneSampler, uvs3p, 0) * weightOffset.z;
    out_Color += texelFetch(SceneSampler, uvs3n, 0) * weightOffset.z;
    out_Color += texelFetch(SceneSampler, uvs4p, 0) * weightOffset.w;
    out_Color += texelFetch(SceneSampler, uvs4n, 0) * weightOffset.w;
}