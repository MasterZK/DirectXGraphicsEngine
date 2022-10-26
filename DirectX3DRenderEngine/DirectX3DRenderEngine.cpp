#include <string>
#include <Windows.h>
#include "WindowsWindow.h"
#include "Direct3D.h"
#include "DirectWriter.h"
#include "DirectInput.h"
#include "Mesh.h"
#include "Camera.h"
#include "Time.h"
#include "Light.h"
#include "Material.h"

//program entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)
{
	int width = 1920 /2;
	int height = 1080 /2;
	bool isWindowed = true;

	// 1. create window
	WindowsWindow window;
	int errorCode = window.init(hInstance, width, height, nCmdShow);
	if (errorCode > 0) return errorCode;

	// 2. create Direct3D 11
	DirectX3D d3d;
	errorCode = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (errorCode > 0) return errorCode;
	
	// create DWrite
	//DirectWriter writer;
	//errorCode = writer.init(d3d.getSwapChain(),width, height);
	//if (errorCode > 0) return errorCode;
	
	// create DirectInput
	DirectInput directInput;
	errorCode = directInput.init(hInstance, window.getWindowHandle());
	if (errorCode > 0) return errorCode;
	
	// 3. create Mesh
	Mesh meshCube;
	errorCode = meshCube.init(d3d.getDevice(), false, Shape::Cube, XMFLOAT3(0, 0, 0));
	if (errorCode > 0) return errorCode;
	
	Mesh meshRect;
	errorCode = meshRect.init(d3d.getDevice(), false, Shape::rectangle, XMFLOAT3(0, 0, 0), 2.0f, 1.0f, 1.0f);
	if (errorCode > 0) return errorCode;
	
	Mesh meshRect2;
	errorCode = meshRect.init(d3d.getDevice(), false, Shape::rectangle, XMFLOAT3(1, 0, 1), 0.5f, 2.0f, 0.5f);
	if (errorCode > 0) return errorCode;
	
	Mesh meshSphere;
	errorCode = meshSphere.init(d3d.getDevice(), true, Shape::Sphere, 2, 30, 30);
	if (errorCode > 0) return errorCode;
	
	// 4. create camera
	Camera camera;
	errorCode = camera.init(width, height, 90.0f);
	if (errorCode > 0) return errorCode;
	
	// 5. create time
	Time time;
	errorCode = time.init();
	if (errorCode > 0) return errorCode;
	
	// 6. create Materials
	Material material;
	errorCode = material.init(d3d.getDevice(), L"brickwall.jpg", true, L"ColorVertexShader.hlsl",L"ColorPixelShader.hlsl");
	if (errorCode > 0) return errorCode;
	
	Material material2;
	errorCode = material2.init(d3d.getDevice(), L"brickwall.jpg ");
	if (errorCode > 0) return errorCode;
	
	Material material3;
	errorCode = material3.init(d3d.getDevice(), L"brickwall.jpg ", true, L"TextureVertexShader.hlsl", L"TexturePixelShader.hlsl");
	if (errorCode > 0) return errorCode;
	
	// 7. create light
	Light light;
	light.position = { 0, 0, 0 };
	light.lightDirection = { -1.0f, -1.0f, 1.0f }; // ATTENTION: not normalized
	light.lightIntensity = 1.0f;
	light.ambientLight = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.color = { 0, 0, 0, 1 };
	
	material.setLight(d3d.getDeviceContext(), light, camera.GetCameraData());
	material.setShaderDataBuffer(d3d.getDeviceContext(), 0.5, 1, 0.25, 10);
	
	meshCube.setPosition(-1, 0, 2);
	meshSphere.setPosition(1, 0, 2);
	meshRect.setPosition(1, 0, 5);
	meshRect2.setPosition(-2, 0, 2);
	
	while (true)
	{
		if (!window.run()) break;
	
		time.update();
	
		d3d.beginScene(1, 1, 0);
		directInput.cameraMovement(&camera,time.getDeltaTime());
		
		//meshCube.update(time.getDeltaTime());
		//material.render(d3d.getDeviceContext(), meshCube.getWorldMatrix(), camera.GetViewProjectionMatrix());
		//d3d.getDeviceContext()->RSSetState(d3d.getRasterizerStateSolid());
		//meshCube.render(d3d.getDeviceContext());
		//d3d.getDeviceContext()->DrawIndexed(meshCube.getIndexCount(), 0, 0);
		
		meshRect.update(time.getDeltaTime());
		material2.render(d3d.getDeviceContext(), meshRect.getWorldMatrix(), camera.GetViewProjectionMatrix());
		d3d.getDeviceContext()->RSSetState(d3d.getRasterizerStateSolid());
		meshRect.render(d3d.getDeviceContext());
		d3d.getDeviceContext()->DrawIndexed(meshRect.getIndexCount(), 0, 0);
		
		//meshSphere.update(time.getDeltaTime());
		//material2.render(d3d.getDeviceContext(), meshSphere.getWorldMatrix(), camera.GetViewProjectionMatrix());
		//d3d.getDeviceContext()->RSSetState(d3d.getRasterizerStateWireframe());
		//meshSphere.render(d3d.getDeviceContext());
		//d3d.getDeviceContext()->DrawIndexed(meshSphere.getIndexCount(), 0, 0);
		
		meshRect2.update(time.getDeltaTime());
		material3.render(d3d.getDeviceContext(), meshRect2.getWorldMatrix(), camera.GetViewProjectionMatrix());
		d3d.getDeviceContext()->RSSetState(d3d.getRasterizerStateWireframe());
		meshRect2.render(d3d.getDeviceContext());
		d3d.getDeviceContext()->DrawIndexed(meshRect2.getIndexCount(), 0, 0);
	
		d3d.endScene();
	}
	
	material2.deInit();
	material.deInit();
	
	time.deInit();
	camera.deInit();
	
	meshSphere.deInit();
	meshRect2.deInit();
	meshRect.deInit();
	meshCube.deInit();
	
	directInput.deinit();
	d3d.deInit();
	window.deInit();


	return 0;
}
