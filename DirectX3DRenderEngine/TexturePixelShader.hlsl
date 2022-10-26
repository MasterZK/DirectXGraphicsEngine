Texture2D MainTexture;
SamplerState MainTextureSampler;

float4 PS(float4 position : SV_POSITION, float2 uv : TEXCOORD, float4 color : COLOR) : SV_TARGET
{
    return MainTexture.Sample(MainTextureSampler, uv); // texture color
    //return MainTexture.Sample(MainTextureSampler, uv) * float4(1.0f, 1.0f, 0.0f, 1.0f); // texture color * main color
    //return MainTexture.Sample(MainTextureSampler, uv) * color; // texture color * vertex color
    //return 1.0f - MainTexture.Sample(MainTextureSampler, uv); // invert texture color
    //return MainTexture.Sample(MainTextureSampler, uv) * float4(0.0f, 1.0f, 1.0f, 1.0f); // texture color without red
    //return MainTexture.Sample(MainTextureSampler, uv) * 2.0f; // texture color x 2
}