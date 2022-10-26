cbuffer MatrixBuffer
{
    float4x4 WorldViewProjection;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer ShaderDataBuffer : register(b1)
{
    float shaderValue1;
    float shaderValue2;
    float shaderValue3;
};

VertexOutput VS( float4 position : POSITION, float4 color : COLOR )
{
	VertexOutput output;
    position.w = 1.0f;
    output.position = mul(position, WorldViewProjection);
    output.color = float4(shaderValue1, shaderValue2, shaderValue3,1.0f);

	return output;
}