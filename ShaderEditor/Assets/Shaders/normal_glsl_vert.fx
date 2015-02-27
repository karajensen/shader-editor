////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;
ifdef: BUMP
    in vec3 in_Tangent;
    in vec3 in_Bitangent;
endif

out vec2 ex_UVs;
out vec3 ex_Normal;
out float ex_Depth;
ifdef: BUMP
    out vec3 ex_Tangent;
    out vec3 ex_Bitangent;
endif

uniform mat4 world;
uniform mat4 viewProjection;
uniform float depthNear;
uniform float depthFar;
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;
    ex_Normal = (world * vec4(in_Normal, 0.0)).xyz;

    ifdef: BUMP
        ex_Tangent = (world * vec4(in_Tangent, 0.0)).xyz;
        ex_Bitangent = (world * vec4(in_Bitangent, 0.0)).xyz;
    endif

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
}