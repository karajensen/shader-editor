////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_normal_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Bitangent;
in vec2 ex_NormalUV0;
in vec2 ex_NormalUV1;
in vec2 ex_NormalUV2;
in float ex_Depth;

uniform sampler2D NormalSampler;
uniform float bumpIntensity;
 
void main(void)
{
    vec3 normalTex0 = texture(NormalSampler, ex_NormalUV0).rgb - 0.5;
    vec3 normalTex1 = texture(NormalSampler, ex_NormalUV1).rgb - 0.5;
    vec3 normalTex2 = texture(NormalSampler, ex_NormalUV2).rgb - 0.5;
    vec3 bump = bumpIntensity * (normalTex0 + normalTex1 + normalTex2);
    
    vec3 normal = normalize(ex_Normal);
    vec3 bitangent = normalize(ex_Bitangent);
    vec3 tangent = normalize(ex_Tangent);

    out_Color.rgb = normalize(normal + bump.x * tangent + bump.y * bitangent);
    out_Color.a = ex_Depth;
}