////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - blur_hlsl.fx
////////////////////////////////////////////////////////////////////////////////////////

cbuffer PixelBuffer : register(b0)
{
    float horizontalPass;
    float verticalPass;
    float blurAmount;
    float blurStep;
    float weightMain;
    float4 weightOffset;
};

struct Attributes
{
    float4 position   : SV_POSITION;
    float2 uvs        : TEXCOORD0;
};

struct Outputs
{
    float4 colour : SV_TARGET0;
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

Outputs PShader(Attributes input)
{
    float4 uvSteps = float4(blurStep, blurStep * 2.0, blurStep * 3.0, blurStep * 4.0);
    float4 horizontalStep = uvSteps * horizontalPass;
    float4 verticalStep = uvSteps * verticalPass;
    float blurring = max(1.0, blurAmount * verticalPass);

    int3 uvs = int3(input.uvs.x * WINDOW_WIDTH, input.uvs.y * WINDOW_HEIGHT, 0);
    int3 uvs1p = int3(uvs.x + horizontalStep.x, uvs.y + verticalStep.x, 0);
    int3 uvs1n = int3(uvs.x - horizontalStep.x, uvs.y - verticalStep.x, 0);
    int3 uvs2p = int3(uvs.x + horizontalStep.y, uvs.y + verticalStep.y, 0);
    int3 uvs2n = int3(uvs.x - horizontalStep.y, uvs.y - verticalStep.y, 0);
    int3 uvs3p = int3(uvs.x + horizontalStep.z, uvs.y + verticalStep.z, 0);
    int3 uvs3n = int3(uvs.x - horizontalStep.z, uvs.y - verticalStep.z, 0);
    int3 uvs4p = int3(uvs.x + horizontalStep.w, uvs.y + verticalStep.w, 0);
    int3 uvs4n = int3(uvs.x - horizontalStep.w, uvs.y - verticalStep.w, 0);

    Outputs output;
    output.colour = SceneSampler.Load(uvs, 0) * weightMain;
    output.colour += SceneSampler.Load(uvs1p, 0) * weightOffset.x;
    output.colour += SceneSampler.Load(uvs1n, 0) * weightOffset.x;
    output.colour += SceneSampler.Load(uvs2p, 0) * weightOffset.y;
    output.colour += SceneSampler.Load(uvs2n, 0) * weightOffset.y;
    output.colour += SceneSampler.Load(uvs3p, 0) * weightOffset.z;
    output.colour += SceneSampler.Load(uvs3n, 0) * weightOffset.z;
    output.colour += SceneSampler.Load(uvs4p, 0) * weightOffset.w;
    output.colour += SceneSampler.Load(uvs4n, 0) * weightOffset.w;
    output.colour *= blurring;
    return output;
}