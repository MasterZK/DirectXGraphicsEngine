float4 PS(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	//return float4(1.0f, 0.0f, 1.0f, 1.0f); // solid general color
	return color;
}