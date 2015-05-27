////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_glsl_vert.fx
// Based on nvidia Water Shader: http://nvidia.com/shaderlibrary
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

out float ex_Depth;
out vec2 ex_UVs;
out vec3 ex_PositionWorld;
out vec3 ex_Normal;
out vec3 ex_Tangent;
out vec3 ex_Bitangent;
out vec3 ex_VertToCamera;
out vec2 ex_NormalUV1;
out vec2 ex_NormalUV2;

uniform float depthNear;
uniform float depthFar;
uniform float timer;
uniform float speed;
uniform mat4 world;
uniform mat4 viewProjection;
uniform vec3 cameraPosition;
uniform vec2 uvScale;
uniform vec2 bumpScale;

uniform float waveFrequency[MAX_WAVES];
uniform float waveAmplitude[MAX_WAVES];
uniform float wavePhase[MAX_WAVES];
uniform float waveDirectionX[MAX_WAVES];
uniform float waveDirectionZ[MAX_WAVES];
 
void main(void)
{
    // Sum the waves together
    float time = timer * speed;
    vec4 wavePosition = in_Position;
    vec2 waveDerivative = vec2(0.0, 0.0);
    for (int i = 0; i < MAX_WAVES; i++)
    {
        // Wave equation: y = a * sin(kx-wt+phase)
        vec2 direction = vec2(waveDirectionX[i], waveDirectionZ[i]);
        float component = dot(direction, in_Position.xz) - (waveFrequency[i] * time) + wavePhase[i];
        wavePosition.y += waveAmplitude[i] * sin(component);
        waveDerivative += waveFrequency[i] * waveAmplitude[i] * cos(component) * direction;
    }

    wavePosition = world * wavePosition;
    gl_Position = viewProjection * wavePosition;
    ex_PositionWorld = wavePosition.xyz;
    ex_UVs = in_UVs * uvScale;
    ex_NormalUV1 = ex_UVs * bumpScale.x;
    ex_NormalUV2 = ex_UVs * bumpScale.y;
    ex_Bitangent = vec3(1, waveDerivative.x, 0);
    ex_Tangent = vec3(0, waveDerivative.y, 1);
    ex_Normal = vec3(-waveDerivative.x, 1, -waveDerivative.y);
    ex_VertToCamera = cameraPosition - ex_PositionWorld;

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
}