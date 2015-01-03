/////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_glsl_vert.fx
// Reference: http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html#Ocean
/////////////////////////////////////////////////////////////////////////////////////////////////////


#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;

out vec2 ex_UVs;
out vec3 ex_PositionWorld;
out vec3 ex_Normal;
out vec3 ex_Tangent;
out vec3 ex_Bitangent;
out vec3 ex_VertToCamera;

uniform mat4 viewProjection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float timer;
uniform float speed;
uniform float bumpIntensity;
uniform vec2 bumpVelocity;
uniform vec2 uvScale;

uniform float waveFrequency[MAX_WAVES];
uniform float waveAmplitude[MAX_WAVES];
uniform float wavePhase[MAX_WAVES];
uniform float waveDirectionX[MAX_WAVES];
uniform float waveDirectionZ[MAX_WAVES];
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    ex_UVs = in_UVs;
    ex_PositionWorld = in_Position.xyz;
    ex_Normal = in_Normal;
    ex_Tangent = in_Tangent;
    ex_Bitangent = in_Bitangent;
    ex_VertToCamera = cameraPosition - in_Position.xyz;
}