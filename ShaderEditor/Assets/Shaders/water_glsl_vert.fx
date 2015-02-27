/////////////////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_glsl_vert.fx
/////////////////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

out vec2 ex_UVs;
out vec3 ex_PositionWorld;
out vec3 ex_Normal;
out vec3 ex_Tangent;
out vec3 ex_Bitangent;
out vec3 ex_VertToCamera;
out vec2 ex_NormalUV0;
out vec2 ex_NormalUV1;
out vec2 ex_NormalUV2;

uniform mat4 viewProjection;
uniform vec3 cameraPosition;
uniform float timer;
uniform float speed;
uniform vec2 bumpVelocity;
uniform vec2 uvScale;

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

    gl_Position = viewProjection * wavePosition;
    ex_PositionWorld = wavePosition.xyz;
    ex_Bitangent = vec3(1, waveDerivative.x, 0);
    ex_Tangent = vec3(0, waveDerivative.y, 1);
    ex_Normal = vec3(-waveDerivative.x, 1, -waveDerivative.y);
    ex_VertToCamera = cameraPosition - ex_PositionWorld;

    // Generate UV Coordinates
    vec4 scale = vec4(2.0, 4.0, 8.0, 0.001);
    vec2 uvVelocity = bumpVelocity * timer * scale.w;
    ex_UVs = in_UVs * uvScale;
    ex_NormalUV0 = in_UVs * uvScale + uvVelocity;
    ex_NormalUV1 = in_UVs * uvScale * scale.x + uvVelocity * scale.y;
    ex_NormalUV2 = in_UVs * uvScale * scale.y + uvVelocity * scale.z;
}