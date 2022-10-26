cbuffer MatrixBuffer
{
    float4x4 WorldViewProjection;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

VertexOutput VS(float4 position : POSITION, float2 uv : TEXCOORD, float4 color : COLOR)
{
    VertexOutput output;
    position.w = 1.0f;
    output.position = mul(position, WorldViewProjection);
    output.uv = uv * float2(1.0f, 1.0f) + float2(0.0f, 0.0f); // uv * tiling + offset
    output.color = color;

    return output;
}