////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneTexture;
uniform sampler2DMS NormalTexture;

uniform float fadeAmount;
uniform float sceneAlpha;
uniform float normalAlpha;
uniform float depthAlpha;
uniform vec3 minimumColor;
uniform vec3 maximumColor;

vec4 GetColour(sampler2DMS texSampler, ivec2 uvs)
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
    vec4 finalColor;
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);

    if (sceneAlpha == 1.0)
    {
        finalColor = GetColour(SceneTexture, uvs);
        finalColor.rgb *= maximumColor - minimumColor;
        finalColor.rgb += minimumColor;
    }
    else
    {
        finalColor = GetColour(NormalTexture, uvs);
        finalColor = (finalColor * normalAlpha) + (finalColor.aaaa * depthAlpha);
    }

    out_Color = finalColor * fadeAmount;
}