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
    float4 worldPos : TEXCOORD1;
};

cbuffer ShaderDataBuffer : register(b1)
{
    float shaderValue1;
    float shaderValue2;
    float shaderValue3;
    float shaderValue4;
};

VertexOutput VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VertexOutput output;

    output.position = mul(inPos, WorldViewProjection);

    float4x4 worldInverse = pow(World, -1);
    output.normal = mul(normal, worldInverse);

    output.uv = inTexCoord;

    output.worldPos = mul(inPos, World);

    return output;
}