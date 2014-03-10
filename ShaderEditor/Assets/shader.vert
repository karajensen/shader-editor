ifndefined: FLAT
    varying vec3 Normal;
    varying vec3 WorldViewPos;
    ifdefined: BUMP
        varying vec3 Binormal;
        varying vec3 Tangent;
    endif
endif

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    ifndefined: FLAT
        WorldViewPos = vec3(gl_ModelViewMatrix * gl_Vertex);
        Normal = gl_NormalMatrix * gl_Normal;
        ifdefined: BUMP
            Tangent = gl_NormalMatrix * gl_MultiTexCoord1.xyz;
            Binormal = gl_NormalMatrix * gl_MultiTexCoord2.xyz;
        endif
    endif
}