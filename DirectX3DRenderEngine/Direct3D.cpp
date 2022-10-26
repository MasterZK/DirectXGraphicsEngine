#include "Direct3D.h"

#include <d3d9types.h>

int DirectX3D::init(HWND hWnd, INT width, INT height, bool isWindowed)
{
    // ------create device, device context & swap chain------
    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hWnd;
    desc.SampleDesc.Count = 1;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Windowed = isWindowed;

    D3D_FEATURE_LEVEL supportedLevels[] = { D3D_FEATURE_LEVEL_10_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                                // graphic adapter
        D3D_DRIVER_TYPE_HARDWARE,               // driver type, hardware or software rendering?
        nullptr,                                // reference to software module if driver type is software
        0,                                      // optional flags
        supportedLevels, 1,                     // supported direct 3d versions
        D3D11_SDK_VERSION,                      // api version the application was build with
        &desc, &pD3DSwapChain, &pD3DDevice,
        nullptr,                                // optional chosen feature level
        &pD3DDeviceContext
    );
    if (FAILED(hr)) return 23;

    // ------create render target view, get back buffer texture before------
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = pD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr)) return 25;

    hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
    if (FAILED(hr)) return 27;

    pBackBuffer->Release();
    pBackBuffer = nullptr;

    // ------create depth stencil view------
    ID3D11Texture2D* pDepthStencilBuffer = nullptr;
    D3D11_TEXTURE2D_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    bufferDesc.Width = width;
    bufferDesc.Height = height;
    bufferDesc.ArraySize = 1;
    bufferDesc.SampleDesc.Count = 1;

    hr = pD3DDevice->CreateTexture2D(&bufferDesc, nullptr, &pDepthStencilBuffer);
    if (FAILED(hr)) return 28;

    hr = pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
    if (FAILED(hr)) return 26;

    pDepthStencilBuffer->Release();
    pDepthStencilBuffer = nullptr;

    pD3DDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

    D3D11_RASTERIZER_DESC rsDesc = {};
	
    // ------create cullNone rastorizer state------
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_NONE;

    hr = pD3DDevice->CreateRasterizerState(&rsDesc, &pRasterizerStateNone);
    if (FAILED(hr)) return 29;
	
    // ------create solid rasterizer state------
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_BACK;

    hr = pD3DDevice->CreateRasterizerState(&rsDesc, &pRasterizerStateSolid);
    if (FAILED(hr)) return 29;

	// ------create wireframe rastorizer state------
    rsDesc.FillMode = D3D11_FILL_WIREFRAME;
    rsDesc.CullMode = D3D11_CULL_NONE;

    hr = pD3DDevice->CreateRasterizerState(&rsDesc, &pRasterizerStateWireframe);
    if (FAILED(hr)) return 29;
	
    // ------set viewport------
    viewPort.Width = width;
    viewPort.Height = height;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    pD3DDeviceContext->RSSetViewports(1, &viewPort);
	
    return 0;
}

void DirectX3D::beginScene(FLOAT red, FLOAT green, FLOAT blue)
{
    // clear back buffer with solid color
    const FLOAT color[] = { red, green, blue, 1.0f };
    pD3DDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
    pD3DDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX3D::endScene()
{
    // swap front and back buffer
    pD3DSwapChain->Present(0, 0);
}

void DirectX3D::deInit()
{
	if (pRasterizerStateNone != nullptr)
    {
        pRasterizerStateNone->Release();
        pRasterizerStateNone = nullptr;
    }
	
	 if (pRasterizerStateWireframe != nullptr)
    {
        pRasterizerStateWireframe->Release();
        pRasterizerStateWireframe = nullptr;
    }
	
    if (pRasterizerStateSolid != nullptr)
    {
        pRasterizerStateSolid->Release();
        pRasterizerStateSolid = nullptr;
    }

    if (pDepthStencilView != nullptr)
    {
        pDepthStencilView->Release();
        pDepthStencilView = nullptr;
    }

    if (pRenderTargetView != nullptr)
    {
        pRenderTargetView->Release();
        pRenderTargetView = nullptr;
    }

    if (pD3DSwapChain != nullptr)
    {
        pD3DSwapChain->Release();
        pD3DSwapChain = nullptr;
    }

    if (pD3DDeviceContext != nullptr)
    {
        pD3DDeviceContext->Release();
        pD3DDeviceContext = nullptr;
    }

    if (pD3DDevice != nullptr)
    {
        pD3DDevice->Release();
        pD3DDevice = nullptr;
    }
}
