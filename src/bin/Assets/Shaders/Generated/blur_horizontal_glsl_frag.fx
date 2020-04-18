
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

    out_Color =  texture(SceneSampler, ex_UVs) * 0.224215;
    out_Color += texture(SceneSampler, uvs1p) * 0.201794;
    out_Color += texture(SceneSampler, uvs1n) * 0.201794;
    out_Color += texture(SceneSampler, uvs2p) * 0.123318;
    out_Color += texture(SceneSampler, uvs2n) * 0.123318;
    out_Color += texture(SceneSampler, uvs3p) * 0.040359;
    out_Color += texture(SceneSampler, uvs3n) * 0.040359;
    out_Color += texture(SceneSampler, uvs4p) * 0.022422;
    out_Color += texture(SceneSampler, uvs4n) * 0.022422;
}
