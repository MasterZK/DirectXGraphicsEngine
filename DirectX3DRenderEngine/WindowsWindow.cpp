#include "WindowsWindow.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int WindowsWindow::init(HINSTANCE hInstance, INT width, INT height, int nCmdShow)
{
	WNDCLASSEX wc = {};

	wc.lpszClassName = TEXT("Zayarmoe_DirectX11");
	wc.hInstance = hInstance;

	wc.cbSize = sizeof(WNDCLASSEX);			//Size of our windows class
	wc.cbClsExtra = NULL;					//Extra bytes after our wc structure
	wc.cbWndExtra = NULL;					//Extra bytes after our windows instance
	wc.style = CS_HREDRAW | CS_VREDRAW;		//class styles
	wc.lpfnWndProc = WindowProc;			//Default windows procedure function
	wc.lpszMenuName = NULL;					//Name of the menu attached to our window

	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wc)) return 10;

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	
	RECT rect = { centerScreenX, centerScreenY, centerScreenX + width, centerScreenY + height };
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, style, false);

	windowHandler = CreateWindowEx(NULL,
		wc.lpszClassName, wc.lpszClassName, style,
		centerScreenX, centerScreenY, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!windowHandler) return 15;

	// 5. show window
	ShowWindow(windowHandler, nCmdShow);
	UpdateWindow(windowHandler);
	SetFocus(windowHandler);

	return 0;
}

bool WindowsWindow::run()
{
	MSG msg = {};
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false;

		if (msg.message == WM_SIZING)

			TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void WindowsWindow::deInit()
{
}