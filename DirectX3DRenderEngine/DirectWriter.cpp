#include "DirectWriter.h"

#include <d3d11.h>

int DirectWriter::init(IDXGISwapChain* swapChain, INT width, INT height)
{
	ID2D1Factory* D2DFactory;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), reinterpret_cast<void**>(&D2DFactory));
	if (hr > 0) return hr;

	IDXGISurface* pBackBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&pBackBuffer));

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = { D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED) };
	
	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer, renderTargetProperties, &D2DRenderTarget);
	if (hr > 0) return hr;

	D2DFactory->Release();

	hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &Brush);
	if (hr > 0) return hr;
	layoutRect = D2D1::RectF(0, 0, width, height);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&DWriteFactory));
	if (hr > 0) return hr;

	hr = DWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&TextFormat
	);
	if (hr > 0) return hr;

	hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (hr > 0) return hr;
	hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (hr > 0) return hr;

	return 0;
}

void DirectWriter::RenderText(LPCWSTR text)
{
	D2DRenderTarget->BeginDraw();
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	D2DRenderTarget->DrawText(
		text,
		wcslen(text),
		TextFormat,
		layoutRect,
		Brush
	);

	D2DRenderTarget->EndDraw();
}

void DirectWriter::deInit()
{
	if (TextFormat != nullptr)
	{
		TextFormat->Release();
		TextFormat = nullptr;
	}

	if (Brush != nullptr)
	{
		Brush->Release();
		Brush = nullptr;
	}

	if (D2DRenderTarget != nullptr)
	{
		D2DRenderTarget->Release();
		D2DRenderTarget = nullptr;
	}

	if (DWriteFactory != nullptr)
	{
		DWriteFactory->Release();
		DWriteFactory = nullptr;
	}
}
