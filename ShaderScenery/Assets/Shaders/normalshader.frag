
uniform float FrustumNear;
uniform float FrustumFar;
varying vec3 Normal;
varying float WorldViewPosZ;

void main()
{
    gl_FragColor.rgb = normalize(Normal);

    float cappedZ = max(min(WorldViewPosZ, FrustumFar),0.0);
    gl_FragColor.a = (cappedZ*((FrustumFar-FrustumNear)/FrustumFar))+FrustumNear;
}
