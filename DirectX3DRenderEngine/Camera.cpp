#include "Camera.h"

using namespace DirectX;

int Camera::init(int screenWidth, int screenHeight, float fovDegrees, float nearPlane, float farPlane)
{
	camera.cameraPos = { 0.0f, 0.0f, -1.0f, 0.0f };
	cameraPosVector = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	cameraPosition = { 0.0f, 0.0f, -1.0f };

	XMMATRIX view = XMMatrixLookToLH(
		XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),   // camera position
		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),    // camera forward direction
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)     // camera up direction
	);
	XMStoreFloat4x4(&viewMatrix, view);

	auto projection = setProjectionMatrix(fovDegrees, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), nearPlane, farPlane);

	XMStoreFloat4x4(&viewProjectionMatrix, view * projection);

	return 0;
}

XMMATRIX Camera::setProjectionMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	XMMATRIX projection = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearPlane, farPlane);

	XMStoreFloat4x4(&projectionMatrix, projection);
	return projection;
}

void Camera::render()
{
	//Calculate new camera rotation matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(cameraRotation.x, cameraRotation.y, cameraRotation.z);

	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, cameraRotationMatrix);

	//Adjust cam target to be offset by the camera's current position
	camTarget += GetPositionVector();

	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, cameraRotationMatrix);

	//Rebuild view matrix
	XMMATRIX view = {};
	view = XMMatrixLookAtLH(GetPositionVector(), camTarget, upDir);
	XMStoreFloat4x4(&viewMatrix, view);

	XMMATRIX vectorRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, cameraRotation.y, 0.0f);
	currentForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vectorRotationMatrix);
	currentBackward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vectorRotationMatrix);
	currentLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vectorRotationMatrix);
	currentRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vectorRotationMatrix);
}

void Camera::lookAtPosition(XMFLOAT3 positionToLookAt)
{
	//check if look at pos is the same as cam pos
	if (positionToLookAt.x == cameraPosition.x && positionToLookAt.y == cameraPosition.y && positionToLookAt.z == cameraPosition.x)
		return;

	positionToLookAt = {
		cameraPosition.x - positionToLookAt.x,
		cameraPosition.y - positionToLookAt.y,
		cameraPosition.z - positionToLookAt.z,
	};

	float pitch = 0.0f;
	if (positionToLookAt.y != 0.0f)
	{
		float distance = sqrt(positionToLookAt.x * positionToLookAt.x + positionToLookAt.z * positionToLookAt.z);
		pitch = atan(positionToLookAt.y / distance);
	}

	float yaw = 0.0f;
	if (positionToLookAt.x != 0.0f)
		yaw = atan(positionToLookAt.x / positionToLookAt.z);
	if (positionToLookAt.z > 0)
		yaw += XM_PI;

	setRotation(pitch, yaw, 0.0f);
}


void Camera::deInit()
{
}

void Camera::setPosition(XMFLOAT3 position)
{
	cameraPosition = position;
	render();
}

void Camera::adjustPosition(XMFLOAT3 positionChange)
{
	cameraPosition.x += positionChange.x;
	cameraPosition.y += positionChange.y;
	cameraPosition.z += positionChange.z;

	cameraPosVector = XMLoadFloat3(&cameraPosition);
	render();
}

void Camera::adjustPosition(XMVECTOR positionChange)
{
	cameraPosVector += positionChange;
	XMStoreFloat3(&cameraPosition, cameraPosVector);
	render();
}

void Camera::setRotation(XMFLOAT3 rotation)
{
	cameraRotation = rotation;
	render();
}

void Camera::adjustRotation(XMFLOAT3 rotationChange)
{
	cameraRotation.x += rotationChange.x;
	cameraRotation.y += rotationChange.y;
	cameraRotation.z += rotationChange.z;
	render();
}

void Camera::adjustRotation(XMVECTOR rotationChange)
{
	XMFLOAT3 rotChange = {};
	XMStoreFloat3(&rotChange, rotationChange);
	cameraRotation.x += rotChange.x;
	cameraRotation.y += rotChange.y;
	cameraRotation.z += rotChange.z;
	render();

}
