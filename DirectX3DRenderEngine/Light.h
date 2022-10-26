#pragma once
#include <DirectXMath.h>

using namespace DirectX;

// values for light source
struct Light
{
	XMFLOAT3 position;
	float padding;
	XMFLOAT3 lightDirection;
	float padding2;
	float lightIntensity;
	float padding3, padding4,padding5;
	XMFLOAT4 ambientLight;
	XMFLOAT4 diffuseLight;
	XMFLOAT4 color;
};
