#version 150

out vec4 out_Color;

uniform sampler2D SceneTexture;
uniform sampler2D NormalTexture;
 
void main(void)
{
    vec4 finalColor = texture(SceneTexture, gl_TexCoord[0].st);
    finalColor.rgb = texture(NormalTexture, gl_TexCoord[0].st).aaa;
    out_Color = finalColor;
}