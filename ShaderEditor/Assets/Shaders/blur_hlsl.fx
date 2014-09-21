////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float blurAmount;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

Texture2DMS<float4,SAMPLES> SceneSampler : register(t0);

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
    //------------------------ Pass this in from code:
    float weight0 = 1.0;
    float weight1 = 0.9;
    float weight2 = 0.55;
    float weight3 = 0.18;
    float weight4 = 0.1;
    float overallWeight = weight0 + 2.0 * (weight1 + weight2 + weight3 + weight4);
    weight0 /= overallWeight;
    weight1 /= overallWeight;
    weight2 /= overallWeight;
    weight3 /= overallWeight;
    weight4 /= overallWeight;
    //-------------------------

    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);
    int3 uvs1p = int3(uvs.x, uvs.y + blurAmount, 0);
    int3 uvs1n = int3(uvs.x, uvs.y - blurAmount, 0);
    int3 uvs2p = int3(uvs.x, uvs.y + blurAmount * 2.0, 0);
    int3 uvs2n = int3(uvs.x, uvs.y - blurAmount * 2.0, 0);
    int3 uvs3p = int3(uvs.x, uvs.y + blurAmount * 3.0, 0);
    int3 uvs3n = int3(uvs.x, uvs.y - blurAmount * 3.0, 0);
    int3 uvs4p = int3(uvs.x, uvs.y + blurAmount * 4.0, 0);
    int3 uvs4n = int3(uvs.x, uvs.y - blurAmount * 4.0, 0);

    float4 finalColor = SceneSampler.Load(uvs, 0) * weight0;
    finalColor += SceneSampler.Load(uvs1p, 0) * weight1;
    finalColor += SceneSampler.Load(uvs1n, 0) * weight1;
    finalColor += SceneSampler.Load(uvs2p, 0) * weight2;
    finalColor += SceneSampler.Load(uvs2n, 0) * weight2;
    finalColor += SceneSampler.Load(uvs3p, 0) * weight3;
    finalColor += SceneSampler.Load(uvs3n, 0) * weight3;
    finalColor += SceneSampler.Load(uvs4p, 0) * weight4;
    finalColor += SceneSampler.Load(uvs4n, 0) * weight4;
    return finalColor;
}