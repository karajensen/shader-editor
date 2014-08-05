////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec2 ex_UVs;
ifndefined: FLAT
    out vec3 ex_VertToLight;
    out vec3 ex_Normal;
    ifdefined: SPECULAR
        out vec3 ex_VertToCamera;
    endif
endif

uniform mat4 viewProjection;
ifndefined: FLAT
    uniform vec3 lightPosition;
    ifdefined: SPECULAR
        uniform vec3 cameraPosition;
    endif
endif
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    ex_UVs = in_UVs;
    
    ifndefined: FLAT
        ex_Normal = in_Normal;
        ex_VertToLight = normalize(lightPosition - in_Position.xyz);
    endif
}