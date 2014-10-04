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
uniform float depthOfFieldMask;
uniform float fogMask;

uniform float dofDistance;
uniform float dofFade;
uniform float glowAmount;
uniform float fadeAmount;
uniform float fogDistance;
uniform float fogFade;
uniform vec3 fogColor;
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
    vec4 blur = texelFetch(BlurSampler, uvs, 0);
    vec3 postScene = scene.rgb;
    float depth = normal.a;

    // Depth of Field
    float dofEnd = dofDistance - dofFade;
    float dofWeight = min(max(((depth-dofEnd)*(1.0/(dofDistance-dofEnd))), 0.0), 1.0);
    vec3 depthOfField = blur.rgb * dofWeight;
    postScene *= (1.0 - dofWeight);
    postScene += depthOfField;

    // Glow
    vec3 postGlow = blur.a * glowAmount * blur.rgb * depth;
    postScene += postGlow;

    // Fog
    float fogEnd = fogDistance + fogFade;
    float fogWeight = min(max(((depth-fogEnd)*(1.0/(fogDistance-fogEnd))), 0.0), 1.0);
    vec3 fog = fogColor * fogWeight;
    postScene *= (1.0 - fogWeight);
    postScene += fog;

    // Colour Correction
    postScene = min(max(postScene, 0.0), 1.0);
    postScene *= maximumColor - minimumColor;
    postScene += minimumColor;

    // Masking the selected texture
    out_Color.rgb = postScene * finalMask;
    out_Color.rgb += scene.rgb * sceneMask;
    out_Color.rgb += normal.rgb * normalMask;
    out_Color.rgb += normal.aaa * depthMask;
    out_Color.rgb += scene.aaa * glowMask;
    out_Color.rgb += postGlow * blurGlowMask;
    out_Color.rgb += blur.rgb * blurSceneMask;
    out_Color.rgb += depthOfField * depthOfFieldMask;
    out_Color.rgb += fog * fogMask;
    out_Color.rgb *= fadeAmount;
}