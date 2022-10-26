#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

enum class Shape
{
	Cube = 1,
	rectangle = 2,
	Sphere = 3,
};

class Mesh
{
public:
	//init constructor for a cube
	int init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, XMFLOAT3 color);
	//init constructor for a rectangle
	int init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, XMFLOAT3 color, float width, float height, float depth);
	//init constructor for a sphere
	int init(ID3D11Device* pD3DDevice, bool AllowInput, Shape shape, float radius, int sliceCount, int stackCount);
	//updates the world matrix of the mesh
	void update(float deltaTime);
	//sets correct buffers
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

	XMFLOAT4X4 getWorldMatrix() { return worldMatrix; }
	UINT getIndexCount() { return indexCount; }

	void setPosition(float x, float y, float z) { position = { x, y, z }; }
	ID3D11Buffer* getVertexBuffer() { return pVertexBuffer; }
	ID3D11Buffer* getIndexBuffer() { return pIndexBuffer; }

private:
	//initializes the vertex buffer for an rectangle
	int initVertexBufferRectangle(ID3D11Device* pD3DDevice, float width, float height, float depth, XMFLOAT3 color);
	//initializes the index buffer for an rectangle
	int initIndexBufferRectangle(ID3D11Device* pD3DDevice);

	//initializes the vertex buffer for an sphere
	int initVertexBufferSphere(ID3D11Device* pD3DDevice, int radius, int sliceCount, int stackCount);
	//initializes the index buffer for an sphere
	int initIndexBufferSphere(ID3D11Device* pD3DDevice, int sliceCount, int stackCount);
	
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	
	UINT vertexCount = 0;
	UINT vertexStride = 0;
	UINT indexCount = 0;

	XMFLOAT4X4 worldMatrix = {};
	XMFLOAT3 position = {};
	XMFLOAT4 rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	
	float angleY = 0.0f;
	float angleX = 0.0f;
	bool allowInput = true;

	XMMATRIX rotationX = {};
	XMMATRIX rotationY = {};
	XMMATRIX rotationZ = {};
};


