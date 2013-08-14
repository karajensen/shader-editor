
varying vec3 Normal;
varying float WorldViewPosZ;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    WorldViewPosZ = (gl_ModelViewMatrix * gl_Vertex).z;
    Normal = gl_NormalMatrix * gl_Normal;
}
