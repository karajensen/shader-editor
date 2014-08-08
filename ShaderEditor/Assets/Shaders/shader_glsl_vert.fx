////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec2 ex_UVs;
out vec3 ex_VertToLight;
ifdef: !FLAT
    out vec3 ex_Normal;
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
    ex_VertToLight = lightPosition - in_Position.xyz;
    
    ifdef: !FLAT
        ex_Normal = in_Normal;
        ifdef: SPECULAR
            ex_VertToCamera = cameraPosition - in_Position.xyz;
        endif
    endif
}