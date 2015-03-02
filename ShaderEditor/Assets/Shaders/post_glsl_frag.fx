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
uniform float blurSceneMask;
uniform float depthOfFieldMask;
uniform float fogMask;

uniform float contrast;
uniform float saturation;
uniform float dofDistance;
uniform float dofFade;
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
    float dofWeight = clamp(((depth-dofEnd)*(1.0/(dofDistance-dofEnd))), 0.0, 1.0);
    vec3 depthOfField = blur.rgb * dofWeight;
    postScene *= (1.0 - dofWeight);
    postScene += depthOfField;

    // Fog
    float fogEnd = fogDistance + fogFade;
    float fogWeight = clamp(((depth-fogEnd)*(1.0/(fogDistance-fogEnd))), 0.0, 1.0);
    vec3 fog = fogColor * fogWeight;
    postScene *= (1.0 - fogWeight);
    postScene += fog;

    // Colour Correction
    postScene = min(max(postScene, 0.0), 1.0);
    postScene *= maximumColor - minimumColor;
    postScene += minimumColor;

    // Contrast and saturation
    // Reference: Programming vertex, geometry and pixel shaders p378-379
    float luminance = dot(postScene.rgb, vec3(0.2126, 0.7152, 0.0722));
    postScene.rgb = mix(vec3(luminance, luminance, luminance), postScene.rgb, saturation);
    postScene -= contrast * (postScene - 1.0) * postScene * (postScene - 0.5);

    // Masking the selected texture
    out_Color.rgb = postScene * finalMask;
    out_Color.rgb += scene.rgb * sceneMask;
    out_Color.rgb += normal.rgb * normalMask;
    out_Color.rgb += normal.aaa * depthMask;
    out_Color.rgb += blur.rgb * blurSceneMask;
    out_Color.rgb += depthOfField * depthOfFieldMask;
    out_Color.rgb += fog * fogMask;
    out_Color.rgb *= fadeAmount;
}