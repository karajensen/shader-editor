#version 150

out vec4 out_Color;

uniform sampler2DMS SceneTexture;
//uniform sampler2DMS NormalTexture;
 
void main(void)
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 size = textureSize(SceneTexture);
    ivec2 uvs = ivec2(size.x * gl_TexCoord[0].s, size.y * gl_TexCoord[0].t);

    //for (int i = 0; i < SAMPLES; ++i)
    {
        finalColor += texelFetch(SceneTexture, uvs, 0);
    }

    finalColor *= 1.0 / SAMPLES;
    //finalColor.a = texelFetch(NormalTexture, uvs, 0).a;

    out_Color = finalColor;
}