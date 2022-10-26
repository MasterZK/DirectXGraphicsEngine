cbuffer MatrixBuffer
{
    float4x4 WorldViewProjection;
    float4x4 World;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

VertexOutput VS(float4 position : POSITION, float2 uv : TEXCOORD, float4 normal : NORMAL)
{
    VertexOutput output;
    position.w = 1.0f;
    output.position = mul(position, WorldViewProjection);
    output.uv = uv * float2(1.0f, 1.0f) + float2(0.0f, 0.0f); // uv * tiling + offset

    normal.w = 0.0f;
    output.normal = mul(normal, World).xyz;
  

    return output;
}