#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneTexture;
uniform sampler2DMS NormalTexture;
uniform float fadeAmount;
 
void main(void)
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);

    for (int i = 0; i < SAMPLES; ++i)
    {
        finalColor += texelFetch(SceneTexture, uvs, i);
    }

    finalColor *= 1.0 / SAMPLES;

    finalColor.a = texelFetch(NormalTexture, uvs, 0).a;

    out_Color = finalColor * fadeAmount;
}