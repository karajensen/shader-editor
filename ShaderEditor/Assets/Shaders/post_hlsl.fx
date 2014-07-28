
cbuffer PixelBuffer
{
    float fadeAmount;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
    float fade        : TEXCOORD1;
};

Texture2DMS<float4,SAMPLES> SceneTexture   : register(t0);
Texture2DMS<float4,SAMPLES> NormalTexture  : register(t1);

Attributes VShader(float4 position  : POSITION,
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = position;
    output.uvs = uvs;
    output.fade = fadeAmount;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColor = float4(0.0, 0.0, 0.0, 0.0);
    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);

    for (int i = 0; i < SAMPLES; ++i)
    {
        finalColor += SceneTexture.Load(uvs, i);
    }
    finalColor *= 1.0 / SAMPLES;

    return finalColor * input.fade;
}