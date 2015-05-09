////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_horizontal_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;

out vec4 out_Color;

uniform sampler2D SceneSampler;

uniform float blurStep;

void main(void)
{
    vec4 uvSteps = vec4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    vec2 uvs1p = vec2(ex_UVs.x + uvSteps.x, ex_UVs.y);
    vec2 uvs1n = vec2(ex_UVs.x - uvSteps.x, ex_UVs.y);
    vec2 uvs2p = vec2(ex_UVs.x + uvSteps.y, ex_UVs.y);
    vec2 uvs2n = vec2(ex_UVs.x - uvSteps.y, ex_UVs.y);
    vec2 uvs3p = vec2(ex_UVs.x + uvSteps.z, ex_UVs.y);
    vec2 uvs3n = vec2(ex_UVs.x - uvSteps.z, ex_UVs.y);
    vec2 uvs4p = vec2(ex_UVs.x + uvSteps.w, ex_UVs.y);
    vec2 uvs4n = vec2(ex_UVs.x - uvSteps.w, ex_UVs.y);

    out_Color =  texture(SceneSampler, ex_UVs) * WEIGHT0;
    out_Color += texture(SceneSampler, uvs1p) * WEIGHT1;
    out_Color += texture(SceneSampler, uvs1n) * WEIGHT1;
    out_Color += texture(SceneSampler, uvs2p) * WEIGHT2;
    out_Color += texture(SceneSampler, uvs2n) * WEIGHT2;
    out_Color += texture(SceneSampler, uvs3p) * WEIGHT3;
    out_Color += texture(SceneSampler, uvs3n) * WEIGHT3;
    out_Color += texture(SceneSampler, uvs4p) * WEIGHT4;
    out_Color += texture(SceneSampler, uvs4n) * WEIGHT4;
}