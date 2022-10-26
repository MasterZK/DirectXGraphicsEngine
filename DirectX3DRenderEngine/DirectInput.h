#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <dinput.h>
#include "Camera.h"

#pragma comment(lib, "dinput8.lib")

using namespace DirectX;

class DirectInput
{
public:
	int init(HINSTANCE hInstance, HWND hwnd);
	//detects mouse movement and returns movement as an float3
	XMFLOAT3 detectMouseInput();
	//checks for wasd input to move the camera
	void cameraMovement(Camera* camera, float deltaTime);
	void deinit();


private:
	IDirectInputDevice8* DIMouse = nullptr;
	IDirectInputDevice8* DIKeyboard = nullptr;

	DIMOUSESTATE mouseLastState = {};
	LPDIRECTINPUT8 DInput = {};
};

