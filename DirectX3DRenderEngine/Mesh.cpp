#include "Mesh.h"
#include <iostream>
#include <DirectXMath.h>

#define M_PI  3.14159265358979323846  /* pi */

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	//XMFLOAT4 worldPos = {0,0,0,0};

	Vertex() : position(0, 0, 0), color(0.0f, 0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vertex(float x, float y, float z) : position(x, y, z), color(0.0f, 0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vertex(float x, float y, float z, float r, float g, float b) : position(x, y, z), color(r, g, b, 1.0f), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : position(x, y, z), color(0.0f, 0.0f, 0.0f, 0.0f), normal(nx, ny, nz), uv(u, v) {}

	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float r, float g, float b) : position(x, y, z), color(r, g, b, 0.0f), normal(nx, ny, nz), uv(u, v) {}
};

int Mesh::init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, XMFLOAT3 color)
{
	allowInput = AllowInput;

	int error = initVertexBufferRectangle(pD3DDevice, 1.0f, 1.0f, 1.0f, color);
	if (error > 0) return error;

	error = initIndexBufferRectangle(pD3DDevice);
	if (error > 0) return error;

	return 0;
}

int Mesh::init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, XMFLOAT3 color, float width, float height, float depth)
{
	allowInput = AllowInput;

	int error = initVertexBufferRectangle(pD3DDevice, width, height, depth, color);
	if (error > 0) return error;

	error = initIndexBufferRectangle(pD3DDevice);
	if (error > 0) return error;

	return 0;
}

int Mesh::init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, float radius, int sliceCount, int stackCount)
{
	allowInput = AllowInput;

	int error = initVertexBufferSphere(pD3DDevice, radius, sliceCount, stackCount);
	if (error > 0) return error;

	error = initIndexBufferSphere(pD3DDevice, sliceCount, stackCount);
	if (error > 0) return error;

	return 0;
}

void Mesh::update(float deltaTime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) angleY -= 1.0f * deltaTime;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) angleY += 1.0f * deltaTime;

	if (GetAsyncKeyState(VK_UP) & 0x8000) angleX += 1.0f * deltaTime;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) angleX -= 1.0f * deltaTime;

	if (angleX > 6.282) angleX = 0;
	if (angleY > 6.282) angleY = 0;

	if (GetAsyncKeyState(0x57) & 0x8000) position.z += 1.0f * deltaTime;
	if (GetAsyncKeyState(0x53) & 0x8000) position.z -= 1.0f * deltaTime;

	if (GetAsyncKeyState(0x41) & 0x8000) position.x += 1.0f * deltaTime;
	if (GetAsyncKeyState(0x44) & 0x8000) position.x -= 1.0f * deltaTime;

	XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rotationX = XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), angleX);
	XMMATRIX rotationY = XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), angleY);
	XMMATRIX _scale = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX world = _scale * rotationX * rotationY * translation;
	XMStoreFloat4x4(&worldMatrix, world);
}

void Mesh::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	static UINT offset = 0;

	pD3DDeviceContext->IASetVertexBuffers(0, 2, &pVertexBuffer, &vertexStride, &offset);
	pD3DDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::deInit()
{
	if (pVertexBuffer != nullptr)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}

	if (pIndexBuffer != nullptr)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}
}

int Mesh::initVertexBufferRectangle(ID3D11Device* pD3DDevice, float width, float height, float depth, XMFLOAT3 color)
{
	vertexCount = 24;
	vertexStride = sizeof(Vertex);

	auto w = width * 0.5f;
	auto h = height * 0.5f;
	auto d = depth * 0.5f;

	Vertex vertices[] =
	{
		// Front Face
		Vertex(-w, -h, -d, 0.0f, 1.0f,  0, 0, -1.0f, color.x,color.y,color.z),
		Vertex(-w,  h, -d, 0.0f, 0.0f,  0, 0, -1.0f, color.x,color.y,color.z),
		Vertex(w,  h, -d, 1.0f, 0.0f,  0, 0, -1.0f, color.x,color.y,color.z),
		Vertex(w, -h, -d, 1.0f, 1.0f,  0, 0, -1.0f, color.x,color.y,color.z),

		// Back Face
		Vertex(-w, -h, d, 1.0f, 1.0f,	0, 0, 1.0f, color.x,color.y,color.z),
		Vertex(w, -h, d, 0.0f, 1.0f,	0, 0, 1.0f, color.x,color.y,color.z),
		Vertex(w,  h, d, 0.0f, 0.0f,	0, 0, 1.0f, color.x,color.y,color.z),
		Vertex(-w,  h, d, 1.0f, 0.0f,	0, 0, 1.0f, color.x,color.y,color.z),

		// Top Face
		Vertex(-w, h, -d, 0.0f, 1.0f, 0, 1.0f, 0, color.x,color.y,color.z),
		Vertex(-w, h,  d, 0.0f, 0.0f, 0, 1.0f, 0, color.x,color.y,color.z),
		Vertex(w, h,  d, 1.0f, 0.0f,  0, 1.0f, 0, color.x,color.y,color.z),
		Vertex(w, h, -d, 1.0f, 1.0f,  0, 1.0f, 0, color.x,color.y,color.z),

		// Bottom Face
		Vertex(-w, -h, -d, 1.0f, 1.0f,0, -1.0f, 0, color.x,color.y,color.z),
		Vertex(w, -h, -d, 0.0f, 1.0f, 0, -1.0f, 0, color.x,color.y,color.z),
		Vertex(w, -h,  d, 0.0f, 0.0f, 0, -1.0f, 0, color.x,color.y,color.z),
		Vertex(-w, -h,  d, 1.0f, 0.0f,0, -1.0f, 0, color.x,color.y,color.z),

		// Left Face
		Vertex(-w, -h,  d, 0.0f, 1.0f, -1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(-w,  h,  d, 0.0f, 0.0f, -1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(-w,  h, -d, 1.0f, 0.0f, -1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(-w, -h, -d, 1.0f, 1.0f, -1.0f, 0, 0, color.x,color.y,color.z),

		// Right Face
		Vertex(w, -h, -d, 0.0f, 1.0f, 1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(w,  h, -d, 0.0f, 0.0f, 1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(w,  h,  d, 1.0f, 0.0f, 1.0f, 0, 0, color.x,color.y,color.z),
		Vertex(w, -h,  d, 1.0f, 1.0f, 1.0f, 0, 0, color.x,color.y,color.z),
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = vertexCount * vertexStride; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &pVertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int Mesh::initIndexBufferRectangle(ID3D11Device* pD3DDevice)
{
	indexCount = 36;

	WORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = indexCount * sizeof(WORD); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &pIndexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int Mesh::initVertexBufferSphere(ID3D11Device* pD3DDevice, int radius, int sliceCount, int stackCount)
{
	const int arraySize = ((sliceCount - 2) * stackCount) + 2;
	auto* vertices = new Vertex[arraySize];

	vertexCount = arraySize;
	vertexStride = sizeof(Vertex);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = radius;

	for (DWORD i = 0; i < sliceCount - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (sliceCount - 1));
		rotationX = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < stackCount; ++j)
		{
			sphereYaw = j * (6.28 / (stackCount));
			rotationY = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotationX * rotationY));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * stackCount + j + 1].normal.x = XMVectorGetX(currVertPos);
			vertices[i * stackCount + j + 1].normal.y = XMVectorGetY(currVertPos);
			vertices[i * stackCount + j + 1].normal.z = XMVectorGetZ(currVertPos);
			
			currVertPos = XMVector3Normalize(currVertPos) * radius;
			vertices[i * stackCount + j + 1].position.x = XMVectorGetX(currVertPos);
			vertices[i * stackCount + j + 1].position.y = XMVectorGetY(currVertPos);
			vertices[i * stackCount + j + 1].position.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[arraySize - 1].position.x = 0.0f;
	vertices[arraySize - 1].position.y = 0.0f;
	vertices[arraySize - 1].position.z = -radius;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = vertexStride * vertexCount ; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &pVertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int Mesh::initIndexBufferSphere(ID3D11Device* pD3DDevice, int sliceCount, int stackCount)
{
	const int arraySize = ((sliceCount - 3) * (stackCount) * 2) + (stackCount * 2);
	auto* indices = new WORD[arraySize * 3];

	indexCount = arraySize*3;

	int k = 0;
	for (DWORD l = 0; l < stackCount - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = stackCount;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < sliceCount - 3; ++i)
	{
		for (DWORD j = 0; j < stackCount - 1; ++j)
		{
			indices[k] = i * stackCount + j + 1;
			indices[k + 1] = i * stackCount + j + 2;
			indices[k + 2] = (i + 1) * stackCount + j + 1;

			indices[k + 3] = (i + 1) * stackCount + j + 1;
			indices[k + 4] = i * stackCount + j + 2;
			indices[k + 5] = (i + 1) * stackCount + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * stackCount) + stackCount;
		indices[k + 1] = (i * stackCount) + 1;
		indices[k + 2] = ((i + 1) * stackCount) + stackCount;

		indices[k + 3] = ((i + 1) * stackCount) + stackCount;
		indices[k + 4] = (i * stackCount) + 1;
		indices[k + 5] = ((i + 1) * stackCount) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < stackCount - 1; ++l)
	{
		indices[k] = vertexCount - 1;
		indices[k + 1] = (vertexCount - 1) - (l + 1);
		indices[k + 2] = (vertexCount - 1) - (l + 2);
		k += 3;
	}

	indices[k] = vertexCount - 1;
	indices[k + 1] = (vertexCount - 1) - stackCount;
	indices[k + 2] = vertexCount - 2;


	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = indexCount * sizeof(WORD); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &pIndexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}
