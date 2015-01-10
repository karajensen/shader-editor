/////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_glsl_frag.fx
// Reference: http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html#Ocean
/////////////////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Bitangent;
in vec3 ex_VertToCamera;

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;

uniform vec3 fresnal;
uniform vec3 reflectionTint;
uniform float reflectionIntensity;
uniform float deepColor;
uniform float shallowColor;

uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform vec3 lightSpecular[MAX_LIGHTS];
uniform float lightSpecularity[MAX_LIGHTS];

void main(void)
{
    out_Color.rgb = texture(DiffuseSampler, ex_UVs).rgb;
    out_Color.a = texture(NormalSampler, ex_UVs).a;
}