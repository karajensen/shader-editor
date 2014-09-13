////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;
ifdef: !FLAT|BUMP
    in vec3 in_Tangent;
    in vec3 in_Bitangent;
endif

out vec2 ex_UVs;
out vec3 ex_PositionWorld;
ifdef: !FLAT
    out vec3 ex_Normal;
    ifdef: BUMP
        out vec3 ex_Tangent;
        out vec3 ex_Bitangent;
    endif
    ifdef: SPECULAR
        out vec3 ex_VertToCamera;
    endif
endif

uniform mat4 viewProjection;
uniform vec3 lightPosition;
ifdef: !FLAT|SPECULAR
    uniform vec3 cameraPosition;
endif
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    ex_UVs = in_UVs;
    ex_PositionWorld = in_Position.xyz;
    
    ifdef: !FLAT
        ex_Normal = in_Normal;
        ifdef: BUMP
            ex_Tangent = in_Tangent;
            ex_Bitangent = in_Bitangent;
        endif

        ifdef: SPECULAR
            ex_VertToCamera = cameraPosition - in_Position.xyz;
        endif
    endif
}