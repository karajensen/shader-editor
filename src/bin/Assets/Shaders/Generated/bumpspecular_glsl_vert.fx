
#version 150

in vec4 in_Position;
in vec2 in_UVs;

in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;

out float ex_Depth;
out vec2 ex_UVs;
out vec3 ex_Normal;
out vec3 ex_PositionWorld;
out vec3 ex_Tangent;
out vec3 ex_Bitangent;
out vec3 ex_VertToCamera;

uniform float depthNear;
uniform float depthFar;
uniform mat4 world;
uniform mat4 viewProjection;
uniform vec3 cameraPosition;
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;

    ex_Normal = (world * vec4(in_Normal, 0.0)).xyz;
    ex_PositionWorld = (world * in_Position).xyz;

    ex_Tangent = (world * vec4(in_Tangent, 0.0)).xyz;
    ex_Bitangent = (world * vec4(in_Bitangent, 0.0)).xyz;

    ex_VertToCamera = cameraPosition - ex_PositionWorld;

    ex_Depth = ((gl_Position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;
}
