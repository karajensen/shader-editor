
struct Attributes
{
    float4 position  : SV_POSITION;
    float2 uvs       : TEXCOORD0;
};

Texture2DMS<float4,4> SceneTexture   : register(t0);
Texture2DMS<float4,4> NormalTexture  : register(t1);

Attributes VShader(float4 position  : POSITION,
                   float2 uvs       : TEXCOORD0)
{
    Attributes output;
    output.position = position;
    output.uvs = uvs;
    return output;
}

float4 PShader(Attributes input) : SV_TARGET
{
    float4 finalColor = float4(0.0, 0.0, 0.0, 0.0);

    //for (int i = 0; i < 4; ++i)
    //{
    //    finalColor += SceneTexture.Load(input.uvs,0);
    //}
    //finalColor /= 4.0;

    finalColor = SceneTexture.Load(input.uvs, 0);

    return finalColor;
}