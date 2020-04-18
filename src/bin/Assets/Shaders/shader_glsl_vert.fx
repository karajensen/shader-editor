////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

ifdef: !FLAT
    in vec3 in_Normal;
endif
ifdef: BUMP
    in vec3 in_Tangent;
    in vec3 in_Bitangent;
endif

out float ex_Depth;
out vec2 ex_UVs;
ifdef: !FLAT
    out vec3 ex_Normal;
    out vec3 ex_PositionWorld;
endif
ifdef: BUMP
    out vec3 ex_Tangent;
    out vec3 ex_Bitangent;
endif
ifdef: SPECULAR
    out vec3 ex_VertToCamera;
endif

uniform float depthNear;
uniform float depthFar;
uniform mat4 world;
uniform mat4 viewProjection;
ifdef: SPECULAR
    uniform vec3 cameraPosition;
endif
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;

    ifdef: !FLAT
        ex_Normal = (world * vec4(in_Normal, 0.0)).xyz;
        ex_PositionWorld = (world * in_Position).xyz;
    endif

    ifdef: BUMP
        ex_Tangent = (world * vec4(in_Tangent, 0.0)).xyz;
        ex_Bitangent = (world * vec4(in_Bitangent, 0.0)).xyz;
    endif

    ifdef: SPECULAR
        ex_VertToCamera = cameraPosition - ex_PositionWorld;
    endif

    ex_Depth = ((gl_Position.z - depthNear) * 
        (-1.0 / (depthFar - depthNear))) + 1.0;
}