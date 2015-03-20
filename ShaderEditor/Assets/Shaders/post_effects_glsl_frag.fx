////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2D SceneSampler;
uniform sampler2D EffectsSampler;
uniform sampler2D BlurSampler;

uniform float bloomIntensity;
uniform float finalMask;
uniform float sceneMask;
uniform float normalMask;
uniform float depthMask;
uniform float blurSceneMask;
uniform float depthOfFieldMask;
uniform float fogMask;
uniform float bloomMask;
uniform float ambienceMask;

uniform float contrast;
uniform float saturation;
uniform float dofStart;
uniform float dofFade;
uniform float fadeAmount;
uniform float fogStart;
uniform float fogFade;
uniform vec3 fogColor;
uniform vec3 minimumColor;
uniform vec3 maximumColor;
 
void main(void)
{
    vec4 scene = texture(SceneSampler, ex_UVs);
    vec4 effects = texture(EffectsSampler, ex_UVs);
    vec4 blur = texture(BlurSampler, ex_UVs);
    vec3 postScene = scene.rgb;
    float depth = effects.a;

    // Ambient Occlusion
    vec3 ambience = effects.rgb;

    // Depth of Field
    float dofEnd = dofStart - dofFade;
    float dofWeight = clamp(((depth-dofEnd)*(1.0/(dofStart-dofEnd))), 0.0, 1.0);
    vec3 depthOfField = blur.rgb * dofWeight;
    postScene *= (1.0 - dofWeight);
    postScene += depthOfField;

    // Fog
    float fogEnd = fogStart + fogFade;
    float fogWeight = clamp(((depth-fogEnd)*(1.0/(fogStart-fogEnd))), 0.0, 1.0);
    vec3 fog = fogColor * fogWeight;
    postScene *= (1.0 - fogWeight);
    postScene += fog;

    // Bloom
    vec3 bloom = blur.rgb * blur.a * bloomIntensity;
    postScene += bloom;

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
    out_Color.rgb += effects.rgb * normalMask;
    out_Color.rgb += effects.aaa * depthMask;
    out_Color.rgb += blur.rgb * blurSceneMask;
    out_Color.rgb += depthOfField * depthOfFieldMask;
    out_Color.rgb += fog * fogMask;
    out_Color.rgb += bloom * bloomMask;
    out_Color.rgb += ambience * ambienceMask;
    out_Color.rgb *= fadeAmount;
}