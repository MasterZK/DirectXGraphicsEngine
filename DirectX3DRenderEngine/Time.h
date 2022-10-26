#pragma once

#pragma comment(lib, "Winmm.lib")

class Time
{
public:
	//init constructor of Time
	int init();
	//calculates delta time in update
	void update();
	void deInit();

	float getDeltaTime() { return deltaTime; }

private:
	float deltaTime = 0.0f;
	float lastTimestamp = 0.0f;
};
