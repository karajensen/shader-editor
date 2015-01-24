/////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_glsl_frag.fx
/////////////////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Bitangent;
in vec3 ex_VertToCamera;
in vec2 ex_NormalUV0;
in vec2 ex_NormalUV1;
in vec2 ex_NormalUV2;

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;
uniform samplerCube EnvironmentSampler;

uniform vec3 fresnal;
uniform vec3 reflectionTint;
uniform float reflectionIntensity;
uniform float deepColor;
uniform float shallowColor;
uniform float bumpIntensity;

uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform vec3 lightSpecular[MAX_LIGHTS];
uniform float lightSpecularity[MAX_LIGHTS];

float saturate(float value)
{
    return min(1.0, max(value, 0.0));
}

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);

    vec3 normalTex0 = texture(NormalSampler, ex_NormalUV0).rgb - 0.5;
    vec3 normalTex1 = texture(NormalSampler, ex_NormalUV1).rgb - 0.5;
    vec3 normalTex2 = texture(NormalSampler, ex_NormalUV2).rgb - 0.5;
    vec3 bump = bumpIntensity * (normalTex0 + normalTex1 + normalTex2);

    vec3 normal = normalize(ex_Normal);
    vec3 bitangent = normalize(ex_Bitangent);
    vec3 tangent = normalize(ex_Tangent);
    normal = normalize(normal + bump.x * tangent + bump.y * bitangent);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec3 lightColour = lightDiffuse[i];
        vec3 vertToLight = lightPosition[i] - ex_PositionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        vertToLight /= lightLength;
        lightColour *= ((dot(vertToLight, normal) + 1.0) * 0.5);
        diffuse.rgb += lightColour * attenuation;
    }

    // Fresnal Approximation = max(0, min(1, bias + scale * (1.0 + dot(I,N))))
    // Reference: NVIDEA CG Chapter 7 Environment Mapping Techniques
    vec3 vertToCamera = normalize(ex_VertToCamera);
    float fresnalFactor = saturate(fresnal.x + fresnal.y * pow(1.0 + dot(-vertToCamera, normal), fresnal.z));

    out_Color = diffuseTex * diffuse;
    out_Color.rgb *= (saturate(dot(vertToCamera, normal))*(deepColor-shallowColor))+shallowColor;

    vec3 reflection = reflect(-vertToCamera, normal);
    vec4 reflectionTex = texture(EnvironmentSampler, reflection);
    out_Color.rgb += reflectionTex.rgb * reflectionTint * reflectionIntensity * fresnalFactor;
    
    out_Color.a = diffuseTex.r * bump.r;
    out_Color.rgb = reflectionTex.rgb;
}