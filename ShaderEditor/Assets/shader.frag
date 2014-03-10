uniform vec4 ComponentVisibility;
uniform sampler2D Sampler0; //Diffuse sampler

ifndefined: FLAT
    varying vec3 Normal;
    varying vec3 WorldViewPos;

    ifdefined: BUMP
        varying vec3 Binormal;
        varying vec3 Tangent;
    endif

    ifdefined: SPECULAR|BUMP
        uniform sampler2D Sampler1; //Bump sampler
        uniform sampler2D Sampler2; //Specular sampler
    elseif: SPECULAR
        uniform sampler2D Sampler1; //Specular sampler
    elseif: BUMP
        uniform sampler2D Sampler1; //Bump sampler
    endif
endif

void main()
{
    int specularVisibility = 0;
    int bumpVisibility = 1;
    int parallaxVisibility = 2;
    int shadowVisibility = 3;

    vec4 finalColor = vec4(0);
    vec4 diffuseTexture = texture2D(Sampler0, gl_TexCoord[0].st);

    ifdefined: FLAT
        finalColor = diffuseTexture;
    else:
        float att;
        float diffuse;
        float lightDist;
        vec3 lightVector;
        vec3 cameraVector = normalize(-WorldViewPos);
    
        ifdefined: BUMP
            vec4 sampledNormal = texture2D(Sampler1, gl_TexCoord[0].st);
            vec3 bump = (sampledNormal.rgb-0.5)*vec3(ComponentVisibility[bumpVisibility]);
            vec3 normNormal = normalize(Normal + bump.x*Tangent + bump.y*Binormal);
        else:
            vec3 normNormal = normalize(Normal);
        endif
    
        ifdefined: SPECULAR
            float specular;
            vec3 halfVector;
            ifdefined: BUMP
                vec4 specularTexture = texture2D(Sampler2, gl_TexCoord[0].st);
            else:
                vec4 specularTexture = texture2D(Sampler1, gl_TexCoord[0].st);
            endif
        endif
    
        for(int i = 0; i < MAX_LIGHTS; ++i)
        {
            lightVector = gl_LightSource[i].position.xyz - WorldViewPos;
            lightDist = length(lightVector);
            lightVector /= lightDist;
    
            float d = gl_LightSource[i].constantAttenuation + 
                gl_LightSource[i].linearAttenuation * lightDist +
                gl_LightSource[i].quadraticAttenuation * lightDist * lightDist;

            float stp = step(d,0.0); // Step = d > 0.0 ? 0.0 : 1.0
            att = mix(1.0/(d+0.00001), 1.0, stp); // prevent d from being 0
            
            diffuse = (dot(lightVector,normNormal) + 1.0) * 0.5; // range between 0->1
            finalColor += diffuse * gl_LightSource[i].diffuse * att;
            
            ifdefined: SPECULAR
                halfVector = normalize(lightVector + cameraVector);
                specular = pow(max(dot(normNormal,halfVector),0.0), gl_FrontMaterial.shininess);
                finalColor += (specular * gl_LightSource[i].specular * specularTexture * att) 
                              * vec4(ComponentVisibility[specularVisibility]);
            endif
        }
        finalColor *= diffuseTexture;
    endif
    
    gl_FragColor = finalColor;
    ifdefined: ALPHA
        gl_FragColor.a = diffuseTexture.a;
    else:
        gl_FragColor.a = 1.0f;
    endif
}