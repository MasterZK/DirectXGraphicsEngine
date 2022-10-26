#include "Material.h"
#include "WICTextureLoader11.h"
#include <d3dcompiler.h>

int Material::init(ID3D11Device* pD3DDevice, LPCWSTR textureName, bool createShaderData, LPCWSTR vertexShader, LPCWSTR pixelShader)
{
	int error = createVertexShader(pD3DDevice, vertexShader);
	if (error > 0) return error;

	error = createPixelShader(pD3DDevice, pixelShader);
	if (error > 0) return error;

	error = createMatrixBuffer(pD3DDevice);
	if (error > 0) return error;

	error = createPixelShaderBuffer(pD3DDevice);
	if (error > 0) return error;

	error = createTextureAndSampler(pD3DDevice, textureName);
	if (error > 0) return error;

	if (createShaderData)
		error = createShaderDataBuffer(pD3DDevice);
	if (error > 0) return error;
	additionalBuffer = createShaderData;

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewProjectionMatrix)
{
	pD3DDeviceContext->IASetInputLayout(pInputLayout);
	pD3DDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(pPixelShader, nullptr, 0);

	setMatrixBuffer(pD3DDeviceContext, worldMatrix, viewProjectionMatrix);

	pD3DDeviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);
	pD3DDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
}

void Material::deInit()
{
	if (pVertexShader != nullptr)
	{
		pVertexShader->Release();
		pVertexShader = nullptr;
	}

	if (pPixelShader != nullptr)
	{
		pPixelShader->Release();
		pPixelShader = nullptr;
	}

	if (pInputLayout != nullptr)
	{
		pInputLayout->Release();
		pInputLayout = nullptr;
	}

	if (pTexture != nullptr)
	{
		pTexture->Release();
		pTexture = nullptr;
	}

	if (pShaderResourceView != nullptr)
	{
		pShaderResourceView->Release();
		pShaderResourceView = nullptr;
	}

	if (pSamplerState != nullptr)
	{
		pSamplerState->Release();
		pSamplerState = nullptr;
	}

	if (pMatrixBuffer != nullptr)
	{
		pMatrixBuffer->Release();
		pMatrixBuffer = nullptr;
	}

	if (pPixelShaderBuffer != nullptr)
	{
		pPixelShaderBuffer->Release();
		pPixelShaderBuffer = nullptr;
	}

	if (pShaderDataBuffer != nullptr)
	{
		pShaderDataBuffer->Release();
		pShaderDataBuffer = nullptr;
	}
}

void Material::setLight(ID3D11DeviceContext* pD3DDeviceContext, const Light& light, const CameraData& camera)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(pPixelShaderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	PixelShaderBuffer* buffer = reinterpret_cast<PixelShaderBuffer*>(data.pData);
	buffer->light = light;
	buffer->camera = camera;

	pD3DDeviceContext->Unmap(pPixelShaderBuffer, 0);

	pD3DDeviceContext->PSSetConstantBuffers(0, 1, &pPixelShaderBuffer);

}

int Material::createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR shadername)
{
	ID3DBlob* pCompiledCode = nullptr;
	HRESULT hr = D3DCompileFromFile(
		shadername, // shader filename
		nullptr, nullptr, // optional macros & includes
		"VS", // entry point function
		"vs_4_0", // shader type & version
		0, 0, // optional flags
		&pCompiledCode, // compiled blob target
		nullptr // optional blob for all compile errors
	);
	if (FAILED(hr)) return 50;

	hr = pD3DDevice->CreateVertexShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr)) return 53;

	auto a = pCompiledCode->GetBufferSize();
	
	int error = createInputLayout(pD3DDevice, pCompiledCode);
	if (error > 0) return error;

	pCompiledCode->Release();
	pCompiledCode = nullptr;

	return 0;
}

int Material::createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR shadername)
{
	ID3DBlob* pCompiledCode = nullptr;
	HRESULT hr = D3DCompileFromFile(shadername,
		nullptr, nullptr,
		"PS", "ps_4_0",
		0, 0, &pCompiledCode, nullptr);
	if (FAILED(hr)) return 57;

	hr = pD3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr)) return 59;

	return 0;
}

int Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[5] = {};

	// position
	elements[0].SemanticName = "POSITION"; // element type
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // element format/size
	elements[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// color
	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// uv
	elements[3].SemanticName = "TEXCOORD";
	elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//uv
	elements[4].SemanticName = "TEXCOORD1";
	elements[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	HRESULT hr = pD3DDevice->CreateInputLayout(elements, 4,
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	if (FAILED(hr)) return 55;

	return 0;
}

int Material::createMatrixBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MatrixBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &pMatrixBuffer);
	if (FAILED(hr)) return 58;

	return 0;
}

int Material::createPixelShaderBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PixelShaderBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &pPixelShaderBuffer);
	if (FAILED(hr)) return 51;

	return 0;
}

int Material::createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName)
{
	// create texture
	HRESULT hr = CreateWICTextureFromFile(pD3DDevice, textureName, &pTexture, &pShaderResourceView);
	if (FAILED(hr)) return 56;

	// create sampler state
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState(&desc, &pSamplerState);
	if (FAILED(hr)) return 54;

	return 0;
}

void Material::setMatrixBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewProjectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX viewProj = XMLoadFloat4x4(&viewProjectionMatrix);
	XMMATRIX wvp = world * viewProj;
	wvp = XMMatrixTranspose(wvp);
	world = XMMatrixTranspose(world);

	MatrixBuffer* buffer = reinterpret_cast<MatrixBuffer*>(data.pData);
	XMStoreFloat4x4(&(buffer->worldViewProjection), wvp);
	XMStoreFloat4x4(&(buffer->world), world);

	pD3DDeviceContext->Unmap(pMatrixBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(0, 1, &pMatrixBuffer);
}

int Material::createShaderDataBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(ShaderDataBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &pShaderDataBuffer);
	if (FAILED(hr)) return hr;

	return 0;
}

void Material::setShaderDataBuffer(ID3D11DeviceContext* pD3DDeviceContext, float shaderValue1, float shaderValue2, float shaderValue3, float shaderValue4)
{
	if (!additionalBuffer)
		return;

	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(pShaderDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	ShaderDataBuffer* buffer = reinterpret_cast<ShaderDataBuffer*>(data.pData);
	buffer->shaderValue1 = shaderValue1;
	buffer->shaderValue2 = shaderValue2;
	buffer->shaderValue3 = shaderValue3;
	buffer->shaderValue4 = shaderValue4;

	pD3DDeviceContext->Unmap(pShaderDataBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(1, 1, &pShaderDataBuffer);
}
