uniform sampler2D Sampler0; //Diffuse Texture
uniform sampler2D Sampler1; //Normal Texture
void main()
{
	vec4 DiffuseTexture = texture2D(Sampler0, gl_TexCoord[0].st);
	vec4 NormalTexture = normalize(texture2D(Sampler1, gl_TexCoord[0].st));



	//gl_FragColor = NormalTexture;
	gl_FragColor = DiffuseTexture;
	//gl_FragColor = vec4(NormalTexture.a, NormalTexture.a, NormalTexture.a, 1.0);
}