#version 150

out vec4 out_Color;
uniform sampler2D SceneTexture;
 
void main(void)
{
    out_Color = texture(SceneTexture, gl_TexCoord[0].st);
}