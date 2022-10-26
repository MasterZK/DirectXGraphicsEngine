#pragma once
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

class DirectX3D
{
public:
	int init(HWND hWnd, INT width, INT height, bool isWindowed);
	void beginScene(FLOAT red, FLOAT green, FLOAT blue);
	void endScene();
	void deInit();

	ID3D11Device* getDevice() { return pD3DDevice; }
	ID3D11DeviceContext* getDeviceContext() { return pD3DDeviceContext; }
	IDXGISwapChain* getSwapChain() { return pD3DSwapChain; }
	ID3D11RasterizerState* getRasterizerStateSolid() { return pRasterizerStateSolid; }
	ID3D11RasterizerState* getRasterizerStateWireframe() { return pRasterizerStateWireframe; }
	ID3D11RasterizerState* getRasterizerStateNone() { return pRasterizerStateNone; }

private:
	ID3D11Device* pD3DDevice = nullptr;								// object for creating direct 3d objects
	ID3D11DeviceContext* pD3DDeviceContext = nullptr;				// object for modify render pipeline
	IDXGISwapChain* pD3DSwapChain = nullptr;						// holds references to front & back buffer and for switching those
	ID3D11RenderTargetView* pRenderTargetView = nullptr;			// reference to back buffer
	ID3D11DepthStencilView* pDepthStencilView = nullptr;			// reference to depth stencil buffer
	ID3D11RasterizerState* pRasterizerStateSolid = nullptr;			// reference to rasterizer stage manipulation
	ID3D11RasterizerState* pRasterizerStateWireframe = nullptr;		// reference to rasterizer stage manipulation
	ID3D11RasterizerState* pRasterizerStateNone = nullptr;			// reference to rasterizer stage manipulation
	D3D11_VIEWPORT viewPort = {};									// hold view port data
};



