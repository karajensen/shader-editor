
uniform float FrustumNear;
uniform float FrustumFar;
varying vec3 Normal;
varying float WorldViewPosZ;

void main()
{
    gl_FragColor.rgb = normalize(Normal);
    gl_FragColor.a = (WorldViewPosZ-FrustumFar)*(1.0f/(FrustumNear-FrustumFar));  
}
