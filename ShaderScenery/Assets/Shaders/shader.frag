#define SPEC_ON 0
#define BUMP_ON 1
#define PARA_ON 2
#define SHAD_ON 3
uniform vec4 Components;

uniform sampler2D Sampler0;
#ifndef FLAT
varying vec3 Normal;
varying vec3 WorldViewPos;
#ifdef BUMP
varying vec3 Binormal;
varying vec3 Tangent;
uniform sampler2D Sampler1;
#endif
#ifdef SPECULAR
#ifdef BUMP
uniform sampler2D Sampler2;
#else
uniform sampler2D Sampler1;
#endif
#endif
#endif

void main()
{
    vec4 finalColor;
    vec4 tex = texture2D(Sampler0, gl_TexCoord[0].st);

#ifdef FLAT
    finalColor = tex;
#else
    finalColor = vec4(0,0,0,0);
    float att;
    float diffuse;
    float lightDist;
    vec3 lightVector;
	vec3 cameraVector = normalize(-WorldViewPos);

	vec3 normNormal = Normal;
#ifdef BUMP
	if(Components[BUMP_ON] == 1.0)
	{
		vec4 sampledNormal = texture2D(Sampler1, gl_TexCoord[0].st);
		vec3 bump = sampledNormal.a*(sampledNormal.rgb-0.5);
		normNormal += bump.x*Tangent + bump.y*Binormal;
	}
#endif
	normNormal = normalize(normNormal);

#ifdef SPECULAR
	vec4 specularColor = vec4(0,0,0,0);
    float specular;
    vec3 halfVector;
#ifdef BUMP
    vec4 specularTexture = texture2D(Sampler2, gl_TexCoord[0].st);
#else
    vec4 specularTexture = texture2D(Sampler1, gl_TexCoord[0].st);
#endif
#endif

    for(int i = 0; i < MAX_LIGHTS; ++i)
    {
        lightVector = gl_LightSource[i].position.xyz - WorldViewPos;
        lightDist = length(lightVector);
        lightVector /= lightDist;

        // Step = d > 0.0 ? 0.0 : 1.0
        float d = gl_LightSource[i].constantAttenuation + 
            gl_LightSource[i].linearAttenuation * lightDist +
            gl_LightSource[i].quadraticAttenuation * lightDist * lightDist;
        att = mix(1.0/(d+0.00001), 1.0, step(d,0.0)); // prevent d from being 0
        
        diffuse = (dot(lightVector,normNormal) + 1.0) * 0.5; // range between 0->1
        finalColor += diffuse * gl_LightSource[i].diffuse * att;
        
    #ifdef SPECULAR
        halfVector = normalize(lightVector + cameraVector);
        specular = pow(max(dot(normNormal,halfVector),0.0), gl_FrontMaterial.shininess);
        specularColor += specular * gl_LightSource[i].specular * specularTexture * att;
    #endif
    }
#ifdef SPECULAR
	if(Components[SPEC_ON] == 1.0)
	{
		finalColor += specularColor;
	}
#endif
    finalColor *= tex;
#endif

    gl_FragColor = finalColor;
#ifdef ALPHA
    gl_FragColor.a = tex.a;
#else
    gl_FragColor.a = 1.0f;
#endif
}