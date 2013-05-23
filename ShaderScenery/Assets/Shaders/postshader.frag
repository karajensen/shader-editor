#define DIFFUSE_MAP 0
#define NORMAL_MAP 1
#define DEPTH_MAP 2
uniform vec3 OutputTexture;

uniform sampler2D Sampler0; //Diffuse Texture
uniform sampler2D Sampler1; //Normal Texture
void main()
{
	vec4 DiffuseTexture = texture2D(Sampler0, gl_TexCoord[0].st);
	vec4 NormalTexture = normalize(texture2D(Sampler1, gl_TexCoord[0].st));

	vec4 FinalColor = DiffuseTexture;

	if(OutputTexture[DIFFUSE_MAP] == 1.0)
	{
		gl_FragColor = DiffuseTexture;
	}
	else if(OutputTexture[NORMAL_MAP] == 1.0)
	{
		gl_FragColor = NormalTexture;
	}
	else if(OutputTexture[DEPTH_MAP] == 1.0)
	{
		gl_FragColor = vec4(NormalTexture.a, NormalTexture.a, NormalTexture.a, 1.0);
	}
	else
	{
		gl_FragColor = FinalColor;
	}
}