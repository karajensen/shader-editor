////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneSampler;
uniform sampler2DMS NormalSampler;
uniform sampler2DMS BlurSampler;

uniform float finalMask;
uniform float sceneMask;
uniform float normalMask;
uniform float depthMask;
uniform float glowMask;
uniform float blurGlowMask;
uniform float blurSceneMask;

uniform float glowAmount;
uniform float fadeAmount;
uniform vec3 minimumColor;
uniform vec3 maximumColor;

vec4 GetMultisampledColour(sampler2DMS samplerName, ivec2 uvs)
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
    vec4 scene = GetMultisampledColour(SceneSampler, uvs);
    vec4 normal = GetMultisampledColour(NormalSampler, uvs);
    vec4 blur = GetMultisampledColour(BlurSampler, uvs);
    float depth = normal.a;
    vec3 postScene = scene.rgb;

    // Adding glow
    vec3 postGlow = blur.a * glowAmount * blur.rgb * depth;
    postScene.rgb += postGlow;

    // Colour correction
    postScene.rgb *= maximumColor - minimumColor;
    postScene.rgb += minimumColor;

    // Masking the selected texture
    vec3 finalColor = vec3(0,0,0);
    finalColor += postScene * finalMask;
    finalColor += scene.rgb * sceneMask;
    finalColor += normal.rgb * normalMask;
    finalColor += normal.aaa * depthMask;
    finalColor += scene.aaa * glowMask;
    finalColor += postGlow * blurGlowMask;
    finalColor += blur.rgb * blurSceneMask;

    out_Color.rgb = finalColor * fadeAmount;
}