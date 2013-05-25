#define POST_MAP 0
#define DIFFUSE_MAP 1
#define NORMAL_MAP 2
#define DEPTH_MAP 3
uniform vec4 OutputTexture;

#define FOG_VIS 0
#define SSAO_VIS 1
#define GLOW_VIS 2
#define DOF_VIS 3
uniform vec4 ComponentVisibility;

uniform sampler2D Sampler0; //Diffuse Texture
uniform sampler2D Sampler1; //Normal Texture
void main()
{
    vec4 DiffuseTexture = texture2D(Sampler0, gl_TexCoord[0].st);
    vec4 NormalTexture = normalize(texture2D(Sampler1, gl_TexCoord[0].st));
    vec4 DepthTexture = vec4(NormalTexture.a, NormalTexture.a, NormalTexture.a, 1.0);

    vec4 FinalColor = DiffuseTexture;

    gl_FragColor = (vec4(OutputTexture[DIFFUSE_MAP])*DiffuseTexture) + 
        (vec4(OutputTexture[NORMAL_MAP])*NormalTexture) + 
        (vec4(OutputTexture[DEPTH_MAP])*DepthTexture) + 
        (vec4(OutputTexture[POST_MAP])*FinalColor);

}