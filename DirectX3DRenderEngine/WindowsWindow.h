#pragma once
#include <Windows.h>

class WindowsWindow
{
public:
	int init(HINSTANCE hInstance, INT width, INT height, int nCmdShow);
	bool run();
	void deInit();

	HWND getWindowHandle() { return windowHandler; }

private:
	HWND windowHandler = nullptr;
};
