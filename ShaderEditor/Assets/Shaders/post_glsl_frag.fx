#version 150

out vec4 out_Color;
uniform sampler2D SceneTexture;
uniform sampler2D NormalTexture;
 
void main(void)
{
    out_Color = texture(SceneTexture, gl_TexCoord[0].st);
    out_Color.rgb = texture(NormalTexture, gl_TexCoord[0].st).rgb;
}