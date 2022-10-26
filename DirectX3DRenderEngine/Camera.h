#pragma once
#include <DirectXMath.h>

#define DEFAULT_FORWARD_VECTOR  XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
#define DEFAULT_UP_VECTOR  XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
#define DEFAULT_BACKWARD_VECTOR  XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)
#define DEFAULT_LEFT_VECTOR  XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)
#define DEFAULT_RIGHT_VECTOR  XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)

using namespace DirectX;

struct CameraData
{
	XMFLOAT4 cameraPos;
};

class Camera
{
public:
	//init constructor of the camera
	int init(int screenWidth, int screenHeight, float fovDegrees = 90.0f, float nearPlane = 0.3f, float farPlane = 1000.0f);
	//set a new projection matrix/ change fov and aspect ratio
	XMMATRIX setProjectionMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
	//updates the position and rotation of the camera
	void render();
	//changes rotation of camera to look at a certain point
	void lookAtPosition(XMFLOAT3 positionToLookAt);
	void deInit();

	//sets the position of the camera 
	void setPosition(float x, float y, float z) { setPosition(XMFLOAT3(x, y, z)); }
	//sets the position of the camera 
	void setPosition(XMFLOAT3 position);

	//moves the camera by positionChange
	void adjustPosition(float x, float y, float z) { adjustPosition(XMFLOAT3(x, y, z)); }
	//moves the camera by positionChange
	void adjustPosition(XMFLOAT3 positionChange);
	//moves the camera by positionChange
	void adjustPosition(XMVECTOR positionChange);

	//sets the rotation of the camera
	void setRotation(float x, float y, float z) { setRotation(XMFLOAT3(x, y, z)); }
	//sets the rotation of the camera
	void setRotation(XMFLOAT3 rotation);

	//rotates camera by positionChange
	void adjustRotation(float x, float y, float z) { adjustRotation(XMFLOAT3(x, y, z)); }
	//rotates camera by rotationChange
	void adjustRotation(XMFLOAT3 rotationChange);
	//rotates camera by rotationChange
	void adjustRotation(XMVECTOR rotationChange);

	float GetCameraSpeed() { return cameraSpeed; }
	void setCameraSpeed(float newSpeed) { cameraSpeed = newSpeed; }
	
	XMFLOAT4X4 GetViewProjectionMatrix() { return viewProjectionMatrix; }
	XMFLOAT3  GetPosition() { return cameraPosition; }
	XMVECTOR GetPositionVector() { return XMLoadFloat3(&cameraPosition); }
	XMFLOAT3 GetRotation() { return cameraRotation; }
	XMVECTOR GetRotationVector() { return XMLoadFloat3(&cameraRotation); }
	CameraData GetCameraData() { return camera; }

	XMVECTOR GetCurrentForward() { return currentForward; }
	XMVECTOR GetCurrentLeft() { return currentLeft; }
	XMVECTOR GetCurrentRight() { return currentRight; }
	XMVECTOR GetCurrentBackward() { return currentBackward; }

private:
	XMFLOAT3 cameraPosition = {};
	XMVECTOR cameraPosVector = {};
	XMFLOAT3 cameraRotation = {};
	float cameraSpeed = 1.0f;
	
	XMFLOAT4X4 viewMatrix = {};
	XMFLOAT4X4 projectionMatrix = {};
	XMFLOAT4X4 viewProjectionMatrix = {};

	CameraData camera = {};

	XMVECTOR currentForward;
	XMVECTOR currentLeft;
	XMVECTOR currentRight;
	XMVECTOR currentBackward;
};
