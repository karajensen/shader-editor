#ifndef FLAT
varying vec3 Normal;
varying vec3 WorldViewPos;
#ifdef BUMP
varying vec3 Binormal;
varying vec3 Tangent;
#ifdef PARALLAX
//varying vec3 tNormal; // tangent space normal
//varying vec3 tCamera; // tangent space eye
#endif
#endif
#endif

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

#ifndef FLAT
    WorldViewPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    Normal = gl_NormalMatrix * gl_Normal;

#ifdef BUMP
    Tangent = gl_NormalMatrix * gl_MultiTexCoord1.xyz;
    Binormal = gl_NormalMatrix * gl_MultiTexCoord2.xyz;

#ifdef PARALLAX
	//mat3 tangentMat;
	//tangentMat[0] = vec3(gl_ModelViewMatrix * vec4(Tangent,1.0));
	//tangentMat[1] = vec3(gl_ModelViewMatrix * vec4(Binormal,1.0));
	//tangentMat[2] = vec3(gl_ModelViewMatrix * vec4(Normal,1.0));
	//tangentMat = transpose(tangentMat); //world->tangent space matrix
	//tCamera = tangentMat * -WorldViewPos;
	//tNormal = tangentMat * Normal;
#endif
#endif
#endif
}