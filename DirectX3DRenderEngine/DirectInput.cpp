#include "DirectInput.h"

int DirectInput::init(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL);
	if (FAILED(hr)) return hr;

	DInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	DInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	DIMouse->Acquire();
	DIKeyboard->Acquire();
	
	return 0;
}

XMFLOAT3 DirectInput::detectMouseInput()
{
	DIMOUSESTATE currentState;

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&currentState);

	XMFLOAT3 mouseInput = {};
	
	if (currentState.lX != mouseLastState.lX)
		mouseInput.x = currentState.lX;

	if (currentState.lY != mouseLastState.lY)
		mouseInput.y = currentState.lY;
	
	if (currentState.lZ != mouseLastState.lZ)
		mouseInput.z = currentState.lZ;
	
	mouseLastState = currentState;

	return mouseInput;
}

void DirectInput::cameraMovement(Camera* camera, float deltaTime)
{
	BYTE keyboardState[256];

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	camera->setCameraSpeed(2.0f);
	
	if (keyboardState[DIK_A] & 0x8000)
	{
		camera->adjustPosition(camera->GetCurrentLeft() * camera->GetCameraSpeed() * deltaTime);
	}
	if (keyboardState[DIK_W] & 0x8000)
	{
		camera->adjustPosition(camera->GetCurrentForward() * camera->GetCameraSpeed() * deltaTime);
	}
	if (keyboardState[DIK_D] & 0x8000)
	{
		camera->adjustPosition(camera->GetCurrentRight() * camera->GetCameraSpeed() * deltaTime);
	}
	if (keyboardState[DIK_S] & 0x8000)
	{
		camera->adjustPosition(camera->GetCurrentBackward() * camera->GetCameraSpeed() * deltaTime);
	}

	if (keyboardState[DIK_SPACE] & 0x8000)
	{
		camera->adjustPosition(0.0f,camera->GetCameraSpeed() * deltaTime, 0.0f);
	}
	if (keyboardState[DIK_S] & 0x8000)
	{
		camera->adjustPosition(0.0f, -camera->GetCameraSpeed() * deltaTime, 0.0f);
	}
	
}

void DirectInput::deinit()
{
	if (DIMouse != nullptr)
	{
		DIMouse->Unacquire();
		DIMouse->Release();
		DIMouse = nullptr;
	}

	if (DIKeyboard != nullptr)
	{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
		DIKeyboard = nullptr;
	}
}
