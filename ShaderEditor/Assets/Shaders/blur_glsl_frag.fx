////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneTexture;

uniform float blurAmount;

vec4 GetMultisampledColour(sampler2DMS texSampler, ivec2 uvs)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        color += texelFetch(texSampler, uvs, i);
    }
    return color * (1.0 / SAMPLES);
}
 
void main(void)
{
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    out_Color = GetMultisampledColour(SceneTexture, uvs);
    out_Color *= blurAmount;
}