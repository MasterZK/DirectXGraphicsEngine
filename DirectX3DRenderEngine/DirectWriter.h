#pragma once
#include <iostream>
#include <D2D1.h>
#include <dwrite.h>

#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

class DirectWriter
{
public:
	int init(IDXGISwapChain* swapChain, INT width, INT height);
	void RenderText(LPCWSTR text);
	void deInit();
	
private:
	IDWriteFactory* DWriteFactory = nullptr;
	ID2D1RenderTarget* D2DRenderTarget = nullptr;
	ID2D1SolidColorBrush* Brush = nullptr;
	IDWriteTextFormat* TextFormat = nullptr;
	D2D1_RECT_F layoutRect = {};
};

