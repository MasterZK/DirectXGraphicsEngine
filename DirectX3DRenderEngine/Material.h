#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"
#include "Camera.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class Material
{
public:
	// init constructor of material/ takes name of texture, vertex and pixel shader
	int init(ID3D11Device* pD3DDevice, LPCWSTR textureName, bool createShaderData = false, LPCWSTR vertexShader = TEXT("LightVertexShader.hlsl"), LPCWSTR pixelShader = TEXT("LightPixelShader.hlsl"));
	void render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewProjectionMatrix);
	void deInit();
	// sets constant buffer per frame/ pixel shader buffer
	void setLight(ID3D11DeviceContext* pD3DDeviceContext, const Light& light, const CameraData& camera);

	// creates additional constant buffer
	int createShaderDataBuffer(ID3D11Device* pD3DDevice);
	// sets values of additional constant buffer
	void setShaderDataBuffer(ID3D11DeviceContext* pD3DDeviceContext, float shaderValue1, float shaderValue2, float shaderValue3, float shaderValue4);

private:
	// creates vertex shader from file
	int createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR shadername);
	// creates pixel shader from file
	int createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR shadername);
	// creates input layout for shader
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);
	// creates constant buffer per object
	int createMatrixBuffer(ID3D11Device* pD3DDevice);
	// creates constant buffer per frame
	int createPixelShaderBuffer(ID3D11Device* pD3DDevice);
	// creates texture from file and creates sampler state
	int createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);

	// updates constant buffer per object for shader
	void setMatrixBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewProjectionMatrix);

	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;

	ID3D11InputLayout* pInputLayout = nullptr;

	ID3D11Resource* pTexture = nullptr;
	ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;

	//constant buffer per object
	ID3D11Buffer* pMatrixBuffer = nullptr;
	struct MatrixBuffer
	{
		XMFLOAT4X4 worldViewProjection;
		XMFLOAT4X4 world;
	};

	//constant buffer per frame
	ID3D11Buffer* pPixelShaderBuffer = nullptr;
	struct PixelShaderBuffer
	{
		Light light;
		CameraData camera;
		float padding, padding2, padding3, padding4;
	};

	//constant buffer for additional values
	ID3D11Buffer* pShaderDataBuffer = nullptr;
	struct ShaderDataBuffer
	{
		float shaderValue1,shaderValue2, shaderValue3, shaderValue4;
	};
	bool additionalBuffer = false;
};