#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;
ifndefined: FLAT
    out vec3 ex_VertToLight;
    out vec3 ex_Normal;
endif

uniform mat4 viewProjection;
ifndefined: FLAT
    uniform vec3 lightPosition;
endif
 
void main(void)
{
    gl_Position = viewProjection * in_Position;
    gl_TexCoord[0].st = in_UVs;
    
    ifndefined: FLAT
        ex_Normal = in_Normal;
        ex_VertToLight = normalize(lightPosition - in_Position.xyz);
    endif
}